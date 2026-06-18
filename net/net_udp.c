/*
 * net_udp.c - simple UDP echo server on CPU0 (FreeRTOS SMP + lwIP 2.1.1 RAW).
 *
 * Architecture:
 *   - lwIP runs in the BSP in RAW mode (NO_SYS=1, LWIP_SOCKET=0). It is not
 *     thread-safe, so EVERYTHING runs in a single task (prvNetworkTask) pinned to CPU0.
 *   - EMAC = GEM1 (XEMACPS dev 0, base 0xE000C000, IRQ 77). xemac_add() registers
 *     the EMAC ISR via XScuGic_RegisterHandler() into the shared XScuGic_ConfigTable,
 *     which the FreeRTOS vApplicationIRQHandler dispatches. IRQ 77 targets CPU0 by default.
 *   - PHY = VSC8541 (RGMII). The BSP cannot identify it, hence the custom InitVSC8541()
 *     (taken from Vitis/FirmwareEthernet). PHY reset via AXI GPIO (active-low NRST).
 *
 * Test from PC:   nc -u 192.168.1.157 5001   (the datagram is echoed back + printed on UART)
 */

#include <string.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

/* Xilinx */
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps.h"
#include "xgpio.h"
#include "sleep.h"

/* lwIP (RAW API) */
#include "netif/xadapter.h"
#include "lwip/init.h"
#include "lwip/udp.h"
#include "lwip/inet.h"

#include "net_udp.h"
#include "vsc8541_regs.h"

/*-----------------------------------------------------------*/
/* Configuration - change here. */

#define netTASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )
#define netTASK_STACK           ( configMINIMAL_STACK_SIZE * 4 )   /* ~6.4 KB, lwIP RAW callbacks */
#define netCORE0_AFFINITY       ( 1U << 0 )

#define netUDP_SERVER_PORT      ( 5001U )

#define netSTATIC_IP            "192.168.1.157"
#define netSTATIC_MASK          "255.255.255.0"
#define netSTATIC_GW            "192.168.1.1"

/* GEM1 (canonical XEMACPS dev 0) and AXI GPIO for PHY reset. */
#define netEMAC_BASEADDR        XPAR_XEMACPS_0_BASEADDR
#define netEMAC_DEVICE_ID       XPAR_XEMACPS_0_DEVICE_ID
#define netPHY_GPIO_DEVICE_ID   XPAR_AXI_GPIO_PHY_0_DEVICE_ID
#define netPHY_GPIO_CHANNEL     ( 1U )
#define netPHY_NRST_PIN         ( 0U )
#define netPHY_ADDR             ( 0U )   /* VSC8541 MDIO address */

/* Board MAC address (HardPix). Must be unique within the network. */
static unsigned char ucMacAddress[] = { 0x5e, 0x4c, 0x1a, 0x77, 0x7e, 0x0a };

/*-----------------------------------------------------------*/

static struct netif xServerNetif;
static XGpio        xPhyGpio;
static XEmacPs      xEmac;

/*-----------------------------------------------------------*/

/* Active-low NRST via AXI GPIO: assert=1 -> hold PHY in reset, assert=0 -> run. */
static void prvPhyReset( uint8_t ucAssert )
{
    uint32_t ulReg = XGpio_DiscreteRead( &xPhyGpio, netPHY_GPIO_CHANNEL );
    ulReg &= ~( 1U << netPHY_NRST_PIN );
    ulReg |= ( ucAssert ? 0U : 1U ) << netPHY_NRST_PIN;
    XGpio_DiscreteWrite( &xPhyGpio, netPHY_GPIO_CHANNEL, ulReg );
}

/*
 * VSC8541 configuration (RGMII, 100BASE-TX FDX). The BSP configure_IEEE_phy_speed()
 * cannot recognize this PHY and overwrites its registers, so we configure it manually.
 * Taken from Vitis/FirmwareEthernet/src/main.c (same board).
 */
static void prvInitVSC8541( XEmacPs *pxEmac )
{
    u16 usPhyReg;

    /* Verify PHY identity (two reads, to let MDIO settle). */
    XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_PHY_ID1, &usPhyReg );
    usleep( 100 );
    XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_PHY_ID2, &usPhyReg );
    usleep( 100 );

    /* 1. SW reset - must be done before writing Reg 4/9, otherwise the reset overwrites them. */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_MODE_CTRL, VSC8541_MCTRL_RESET );
    do {
        XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_MODE_CTRL, &usPhyReg );
        usleep( 10 );
    } while( usPhyReg & VSC8541_MCTRL_RESET );
    usleep( 100 );

    /* 2. MAC interface = RGMII (Reg 23). */
    usPhyReg = ( VSC8541_EPHYC1_MAC_IF_RGMII << VSC8541_EPHYC1_MAC_IF_SHIFT );
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_EXT_PHY_CTRL1, usPhyReg );
    usleep( 100 );

    /* 2a. SW reset to activate the MAC IF change. */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_MODE_CTRL, VSC8541_MCTRL_RESET );
    do {
        XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_MODE_CTRL, &usPhyReg );
        usleep( 10 );
    } while( usPhyReg & VSC8541_MCTRL_RESET );
    usleep( 100 );

    /* 3. Disable 1G advertisement (Reg 9). */
    XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_1000T_CTRL, &usPhyReg );
    usPhyReg &= ~( VSC8541_1GCTRL_1000T_FDX_ADV | VSC8541_1GCTRL_1000T_HDX_ADV );
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_1000T_CTRL, usPhyReg );
    usleep( 100 );

    /* 4. Advertise 100BASE-TX FDX only. */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_ANEG_ADV,
                      VSC8541_ANEG_100TX_FDX | VSC8541_ANEG_SEL_IEEE8023 );
    usleep( 100 );

    /* 5. Enable and restart autoneg (Reg 0). */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_MODE_CTRL,
                      VSC8541_MCTRL_ANEG_EN | VSC8541_MCTRL_RESTART_ANEG );
    usleep( 100 );

    /* 6. Extended page 2 - RGMII clock delays. */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_PAGE_SEL, VSC8541_PAGE_EXT2 );
    usleep( 100 );
    XEmacPs_PhyRead( pxEmac, netPHY_ADDR, VSC8541_REG_E2_RGMII_CTRL, &usPhyReg );
    usPhyReg &= ~( VSC8541_RGMII_RX_CLK_MASK | VSC8541_RGMII_TX_CLK_MASK );
    usPhyReg |= ( VSC8541_RGMII_CLK_DLY_2_0NS << VSC8541_RGMII_RX_CLK_SHIFT ) |
                ( VSC8541_RGMII_CLK_DLY_2_0NS << VSC8541_RGMII_TX_CLK_SHIFT );
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_E2_RGMII_CTRL, usPhyReg );
    usleep( 100 );

    /* 7. Back to the main page. */
    XEmacPs_PhyWrite( pxEmac, netPHY_ADDR, VSC8541_REG_PAGE_SEL, VSC8541_PAGE_MAIN );
    usleep( 100 );
}

/*-----------------------------------------------------------*/

/* UDP recv callback - runs in the context of prvNetworkTask (CPU0) from xemacif_input(). */
static void prvUdpRecv( void *pvArg, struct udp_pcb *pxPcb, struct pbuf *pxBuf,
                        const ip_addr_t *pxAddr, u16_t usPort )
{
    ( void ) pvArg;

    if( pxBuf == NULL )
    {
        return;
    }

    xil_printf( "[CPU%d UDP] rx %u B od %d.%d.%d.%d:%u\r\n",
                ( int ) portGET_CORE_ID(), ( unsigned ) pxBuf->tot_len,
                ip4_addr1( pxAddr ), ip4_addr2( pxAddr ),
                ip4_addr3( pxAddr ), ip4_addr4( pxAddr ), ( unsigned ) usPort );

    /* Echo: send the received datagram back to the sender. */
    ( void ) udp_sendto( pxPcb, pxBuf, pxAddr, usPort );

    pbuf_free( pxBuf );
}

/*-----------------------------------------------------------*/

static void prvPrintIp( const char *pcMsg, ip_addr_t *pxIp )
{
    xil_printf( "%s%d.%d.%d.%d\r\n", pcMsg,
                ip4_addr1( pxIp ), ip4_addr2( pxIp ),
                ip4_addr3( pxIp ), ip4_addr4( pxIp ) );
}

/*-----------------------------------------------------------*/

static void prvNetworkTask( void *pvParameters )
{
    struct netif    *pxNetif = &xServerNetif;
    XEmacPs_Config  *pxEmacCfg;
    struct udp_pcb  *pxPcb;
    ip_addr_t        xIp, xMask, xGw;

    ( void ) pvParameters;

    /* --- 1. Reset PHY via AXI GPIO --- */
    XGpio_Initialize( &xPhyGpio, netPHY_GPIO_DEVICE_ID );
    XGpio_SetDataDirection( &xPhyGpio, netPHY_GPIO_CHANNEL, ~( 1U << netPHY_NRST_PIN ) );
    prvPhyReset( 1 );
    vTaskDelay( pdMS_TO_TICKS( 100 ) );
    prvPhyReset( 0 );
    vTaskDelay( pdMS_TO_TICKS( 100 ) );

    /* --- 2. EMAC + custom VSC8541 configuration --- */
    pxEmacCfg = XEmacPs_LookupConfig( netEMAC_DEVICE_ID );
    if( pxEmacCfg == NULL )
    {
        xil_printf( "[NET] XEmacPs_LookupConfig failed\r\n" );
        vTaskDelete( NULL );
    }
    XEmacPs_CfgInitialize( &xEmac, pxEmacCfg, pxEmacCfg->BaseAddress );
    prvInitVSC8541( &xEmac );

    /* --- 3. Static IP --- */
    ( void ) inet_aton( netSTATIC_IP,   &xIp );
    ( void ) inet_aton( netSTATIC_MASK, &xMask );
    ( void ) inet_aton( netSTATIC_GW,   &xGw );

    /* --- 4. lwIP init + add netif (registers and enables the EMAC ISR on CPU0) --- */
    lwip_init();

    if( !xemac_add( pxNetif, &xIp, &xMask, &xGw, ucMacAddress, netEMAC_BASEADDR ) )
    {
        xil_printf( "[NET] xemac_add failed\r\n" );
        vTaskDelete( NULL );
    }
    netif_set_default( pxNetif );

    /* xemac_add -> configure_IEEE_phy_speed() overwrote the VSC8541; re-apply
     * our configuration and wait for autoneg to complete (~2 s). */
    prvInitVSC8541( &xEmac );
    vTaskDelay( pdMS_TO_TICKS( 2000 ) );

    netif_set_up( pxNetif );

    xil_printf( "[NET] interface UP (CPU%d)\r\n", ( int ) portGET_CORE_ID() );
    prvPrintIp( "[NET] IP:      ", &( pxNetif->ip_addr ) );
    prvPrintIp( "[NET] Netmask: ", &( pxNetif->netmask ) );
    prvPrintIp( "[NET] Gateway: ", &( pxNetif->gw ) );

    /* --- 5. UDP echo server --- */
    pxPcb = udp_new();
    if( pxPcb == NULL )
    {
        xil_printf( "[NET] udp_new failed\r\n" );
        vTaskDelete( NULL );
    }
    if( udp_bind( pxPcb, IP_ADDR_ANY, netUDP_SERVER_PORT ) != ERR_OK )
    {
        xil_printf( "[NET] udp_bind failed\r\n" );
        vTaskDelete( NULL );
    }
    udp_recv( pxPcb, prvUdpRecv, NULL );

    xil_printf( "[NET] UDP echo server listening on port %u\r\n",
                ( unsigned ) netUDP_SERVER_PORT );

    /* --- 6. Poll loop: moves received frames from the driver into lwIP.
     * vTaskDelay(1) yields CPU0 to the other tasks (RX is driven by the EMAC ISR). */
    for( ;; )
    {
        xemacif_input( pxNetif );
        vTaskDelay( 1 );
    }
}

/*-----------------------------------------------------------*/

void vStartNetworkTask( void )
{
    ( void ) xTaskCreateAffinitySet( prvNetworkTask,
                                     "Net",
                                     netTASK_STACK,
                                     NULL,
                                     netTASK_PRIORITY,
                                     netCORE0_AFFINITY,
                                     NULL );
}
