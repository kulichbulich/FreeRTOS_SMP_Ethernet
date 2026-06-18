/*
 * FreeRTOS SMP firmware - application entry point.
 *
 * Demonstrates SMP core affinity: two tasks are pinned to CPU0 and CPU1 and
 * exchange a message every 500 ms in a ping-pong fashion over two queues.
 *   - prvPingTask runs on CPU0: sends a "ping" and waits for the "pong" reply.
 *   - prvPongTask runs on CPU1: waits for the "ping" and sends a "pong" back.
 */

#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"
#include "xscugic.h"

/* Network (UDP echo server on CPU0). */
#include "net/net_udp.h"

/* Task priorities and stack sizes. */
#define mainPING_PONG_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainTASK_STACK            ( configMINIMAL_STACK_SIZE * 2 )

/* Core affinity masks - one bit per core. */
#define mainCORE0_AFFINITY        ( 1U << 0 )
#define mainCORE1_AFFINITY        ( 1U << 1 )

/* Exchange period. */
#define mainEXCHANGE_PERIOD_MS    ( 500U )

/* Queue depth for each direction. */
#define mainQUEUE_LENGTH          ( 4 )

/* Message passed between the two cores. */
typedef struct
{
    uint32_t ulCounter;       /* Sequence number. */
    char     pcText[ 24 ];    /* Human readable payload. */
} Message_t;

/* Queues: ping (CPU0 -> CPU1) and pong (CPU1 -> CPU0). */
static QueueHandle_t xPingQueue = NULL;
static QueueHandle_t xPongQueue = NULL;

/*-----------------------------------------------------------*/

static void prvSetupHardware( void );
static void prvPingTask( void *pvParameters );
static void prvPongTask( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
    xil_printf( "FreeRTOS SMP firmware starting (%d cores)\r\n", configNUMBER_OF_CORES );

    prvSetupHardware();

    /* Create the queues used to pass messages between the cores. */
    xPingQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( Message_t ) );
    xPongQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( Message_t ) );
    configASSERT( xPingQueue );
    configASSERT( xPongQueue );

    /* Create the ping task pinned to CPU0 and the pong task pinned to CPU1. */
    xTaskCreateAffinitySet( prvPingTask,
                            "Ping",
                            mainTASK_STACK,
                            NULL,
                            mainPING_PONG_PRIORITY,
                            mainCORE0_AFFINITY,
                            NULL );

    xTaskCreateAffinitySet( prvPongTask,
                            "Pong",
                            mainTASK_STACK,
                            NULL,
                            mainPING_PONG_PRIORITY,
                            mainCORE1_AFFINITY,
                            NULL );

    /* Start the UDP echo server task pinned to CPU0 (lwIP RAW mode). */
    vStartNetworkTask();

    /* Start the scheduler - this should never return. */
    vTaskStartScheduler();

    /* Only reached if there was insufficient heap to start the scheduler. */
    for( ;; );

    return 0;
}
/*-----------------------------------------------------------*/

/* Runs on CPU0: drives the exchange every 500 ms. */
static void prvPingTask( void *pvParameters )
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS( mainEXCHANGE_PERIOD_MS );
    Message_t xTx;
    Message_t xRx;
    uint32_t ulCounter = 0;

    ( void ) pvParameters;

    for( ;; )
    {
        /* Build and send a ping to the task on CPU1. */
        xTx.ulCounter = ulCounter++;
        ( void ) strcpy( xTx.pcText, "ping" );

        xil_printf( "[CPU%d Ping] -> send #%u '%s'\r\n",
                    ( int ) portGET_CORE_ID(), ( unsigned ) xTx.ulCounter, xTx.pcText );

        ( void ) xQueueSend( xPingQueue, &xTx, portMAX_DELAY );

        /* Wait for the reply from CPU1. */
        if( xQueueReceive( xPongQueue, &xRx, portMAX_DELAY ) == pdPASS )
        {
            xil_printf( "[CPU%d Ping] <- recv #%u '%s'\r\n",
                        ( int ) portGET_CORE_ID(), ( unsigned ) xRx.ulCounter, xRx.pcText );
        }

        /* Run the exchange once every 500 ms. */
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
    }
}
/*-----------------------------------------------------------*/

/* Runs on CPU1: replies to every ping. */
static void prvPongTask( void *pvParameters )
{
    Message_t xRx;
    Message_t xTx;

    ( void ) pvParameters;

    for( ;; )
    {
        /* Block until a ping arrives from CPU0. */
        if( xQueueReceive( xPingQueue, &xRx, portMAX_DELAY ) == pdPASS )
        {
            xil_printf( "[CPU%d Pong] <- recv #%u '%s'\r\n",
                        ( int ) portGET_CORE_ID(), ( unsigned ) xRx.ulCounter, xRx.pcText );

            /* Send the matching pong back to CPU0. */
            xTx.ulCounter = xRx.ulCounter;
            ( void ) strcpy( xTx.pcText, "pong" );

            xil_printf( "[CPU%d Pong] -> send #%u '%s'\r\n",
                        ( int ) portGET_CORE_ID(), ( unsigned ) xTx.ulCounter, xTx.pcText );

            ( void ) xQueueSend( xPongQueue, &xTx, portMAX_DELAY );
        }
    }
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    XScuGic_Config *pxGICConfig;

    /* Ensure no interrupts execute while the scheduler is in an inconsistent
    state. Interrupts are automatically enabled when the scheduler is
    started. */
    portDISABLE_INTERRUPTS();

    /* Obtain the configuration of the GIC. */
    pxGICConfig = XScuGic_LookupConfig( XPAR_SCUGIC_SINGLE_DEVICE_ID );
    ( void ) pxGICConfig; /* Prevent compiler warning when configASSERT() is not defined. */

    /* Sanity check the FreeRTOSConfig.h settings are correct for the
    hardware. */
    configASSERT( pxGICConfig );
    configASSERT( pxGICConfig->CpuBaseAddress == ( configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ) );
    configASSERT( pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS );
}
