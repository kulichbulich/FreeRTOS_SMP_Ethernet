/* VSC8541-02/-05 PHY Register Map  –  VMDS-10513 Rev 4.2
 *
 * Register page is selected by writing to register 31.
 * Bit macros: single-bit fields are a mask in place (use & directly).
 * Multi-bit fields: _MASK is the mask in place, _SHIFT is the shift.
 *   Extract: (val & FOO_MASK) >> FOO_SHIFT
 *   Insert : (val & ~FOO_MASK) | (((x) << FOO_SHIFT) & FOO_MASK)
 */

#ifndef VSC8541_REGS_H
#define VSC8541_REGS_H

/* ================================================================== */
/* Page selection  –  write to register 31                            */
/* ================================================================== */

#define VSC8541_REG_PAGE_SEL            31u

#define VSC8541_PAGE_MAIN               0x0000u   /* Section 4.2 */
#define VSC8541_PAGE_EXT1               0x0001u   /* Section 4.3 */
#define VSC8541_PAGE_EXT2               0x0002u   /* Section 4.4 */
#define VSC8541_PAGE_GPIO               0x0010u   /* GPIO / General Purpose */

/* ================================================================== */
/* Section 4.2  –  Main Registers  (page 0x0000)                      */
/* ================================================================== */

/* Register addresses */
#define VSC8541_REG_MODE_CTRL           0u
#define VSC8541_REG_MODE_STATUS         1u
#define VSC8541_REG_PHY_ID1             2u
#define VSC8541_REG_PHY_ID2             3u
#define VSC8541_REG_ANEG_ADV            4u
#define VSC8541_REG_ANEG_LP             5u
#define VSC8541_REG_ANEG_EXP            6u
#define VSC8541_REG_ANEG_NP_TX          7u
#define VSC8541_REG_ANEG_LP_NP          8u
#define VSC8541_REG_1000T_CTRL          9u
#define VSC8541_REG_1000T_STATUS        10u
/* 11, 12 reserved */
#define VSC8541_REG_MMD_CTRL            13u
#define VSC8541_REG_MMD_DATA            14u
#define VSC8541_REG_EXT_STATUS          15u   /* Table 34: 1000BASE-T Status Extension 1 */
#define VSC8541_REG_100TX_STAT_EXT      16u   /* Table 35: 100BASE-TX Status Extension (RO) */
#define VSC8541_REG_1000T_STAT_EXT2     17u   /* Table 36: 1000BASE-T Status Extension 2 (RO) */
#define VSC8541_REG_BYPASS_CTRL         18u   /* Table 37: Bypass Control */
#define VSC8541_REG_ERR_CNT1            19u   /* Table 38: Error Counter 1 – clears on read */
#define VSC8541_REG_ERR_CNT2            20u   /* Table 39: Error Counter 2 – clears on read */
#define VSC8541_REG_ERR_CNT3            21u   /* Table 40: Error Counter 3 – clears on read */
#define VSC8541_REG_EXT_CTRL_STAT       22u   /* Table 41: Extended Control and Status */
#define VSC8541_REG_EXT_PHY_CTRL1       23u   /* Table 42: Extended PHY Control 1 */
#define VSC8541_REG_EXT_PHY_CTRL2       24u   /* Table 43: Extended PHY Control 2 */
#define VSC8541_REG_INT_MASK            25u   /* Table 44: Interrupt Mask */
#define VSC8541_REG_INT_STATUS          26u   /* Table 45: Interrupt Status – clears on read */
/* 27 reserved */
#define VSC8541_REG_AUX_CTRL_STAT       28u   /* Table 46: Auxiliary Control and Status */
#define VSC8541_REG_LED_MODE_SEL        29u   /* Table 47: LED Mode Select */
#define VSC8541_REG_LED_BEHAVIOR        30u   /* Table 48: LED Behavior */

/* ------------------------------------------------------------------
 * Reg 0  –  Mode Control  (Table 20)
 * speed field: {bit13=MSB, bit6=LSB}  00=10M  01=100M  10=1000M
 * ------------------------------------------------------------------ */
#define VSC8541_MCTRL_RESET             (1u << 15)  /* soft-reset; self-clears */
#define VSC8541_MCTRL_LOOPBACK          (1u << 14)
#define VSC8541_MCTRL_SPD_MSB           (1u << 13)  /* speed select MSB */
#define VSC8541_MCTRL_ANEG_EN           (1u << 12)
#define VSC8541_MCTRL_POWERDOWN         (1u << 11)
#define VSC8541_MCTRL_ISOLATE           (1u << 10)
#define VSC8541_MCTRL_RESTART_ANEG      (1u <<  9)
#define VSC8541_MCTRL_DUPLEX_FDX        (1u <<  8)  /* 1 = full duplex */
#define VSC8541_MCTRL_COL_TEST          (1u <<  7)
#define VSC8541_MCTRL_SPD_LSB           (1u <<  6)  /* speed select LSB */
/* Both speed bits together (non-contiguous: bit13=MSB, bit6=LSB) */
#define VSC8541_MCTRL_SPD_MASK          (VSC8541_MCTRL_SPD_MSB | VSC8541_MCTRL_SPD_LSB)
/* Speed values – use: (reg & ~VSC8541_MCTRL_SPD_MASK) | VSC8541_MCTRL_SPD_xxx */
#define VSC8541_MCTRL_SPD_10M           (0u)                      /* {0,0} */
#define VSC8541_MCTRL_SPD_100M          (VSC8541_MCTRL_SPD_LSB)   /* {0,1} */
#define VSC8541_MCTRL_SPD_1000M         (VSC8541_MCTRL_SPD_MSB)   /* {1,0} */

/* ------------------------------------------------------------------
 * Reg 1  –  Mode Status  (Table 21)
 * ------------------------------------------------------------------ */
#define VSC8541_MSTS_100T4_CAP          (1u << 15)
#define VSC8541_MSTS_100TX_FDX_CAP      (1u << 14)
#define VSC8541_MSTS_100TX_HDX_CAP      (1u << 13)
#define VSC8541_MSTS_10T_FDX_CAP        (1u << 12)
#define VSC8541_MSTS_10T_HDX_CAP        (1u << 11)
#define VSC8541_MSTS_100T2_FDX_CAP      (1u << 10)
#define VSC8541_MSTS_100T2_HDX_CAP      (1u <<  9)
#define VSC8541_MSTS_EXT_STATUS         (1u <<  8)
#define VSC8541_MSTS_PREAMBLE_SUPR      (1u <<  6)
#define VSC8541_MSTS_ANEG_COMPLETE      (1u <<  5)
#define VSC8541_MSTS_REMOTE_FAULT       (1u <<  4)  /* latch-high */
#define VSC8541_MSTS_ANEG_CAP           (1u <<  3)
#define VSC8541_MSTS_LINK_UP            (1u <<  2)  /* latch-low */
#define VSC8541_MSTS_JABBER             (1u <<  1)  /* latch-high */
#define VSC8541_MSTS_EXT_CAP            (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 2  –  PHY Identifier 1  (OUI[17:2])
 * ------------------------------------------------------------------ */
#define VSC8541_ID1_OUI_MASK            0xFFFFu
#define VSC8541_ID1_OUI_SHIFT           0u

/* ------------------------------------------------------------------
 * Reg 3  –  PHY Identifier 2  (Table 22)
 * ------------------------------------------------------------------ */
#define VSC8541_ID2_OUI_MASK            (0x3Fu << 10)   /* OUI[23:18] */
#define VSC8541_ID2_OUI_SHIFT           10u
#define VSC8541_ID2_MODEL_MASK          (0x3Fu <<  4)
#define VSC8541_ID2_MODEL_SHIFT         4u
#define VSC8541_ID2_REV_MASK            (0x0Fu <<  0)
#define VSC8541_ID2_REV_SHIFT           0u

/* ------------------------------------------------------------------
 * Reg 4  –  Auto-Negotiation Advertisement  (Table 23)
 * Reg 5  –  Auto-Neg Link Partner Base Page Ability  (same layout)
 * ------------------------------------------------------------------ */
#define VSC8541_ANEG_NEXT_PAGE          (1u << 15)
#define VSC8541_ANEG_ACK                (1u << 14)
#define VSC8541_ANEG_REMOTE_FAULT       (1u << 13)
#define VSC8541_ANEG_ASYM_PAUSE         (1u << 11)
#define VSC8541_ANEG_PAUSE              (1u << 10)
#define VSC8541_ANEG_100T4              (1u <<  9)
#define VSC8541_ANEG_100TX_FDX          (1u <<  8)
#define VSC8541_ANEG_100TX_HDX          (1u <<  7)
#define VSC8541_ANEG_10T_FDX            (1u <<  6)
#define VSC8541_ANEG_10T_HDX            (1u <<  5)
#define VSC8541_ANEG_SEL_MASK           (0x1Fu << 0)
#define VSC8541_ANEG_SEL_SHIFT          0u
#define VSC8541_ANEG_SEL_IEEE8023       0x01u

/* ------------------------------------------------------------------
 * Reg 6  –  Auto-Neg Expansion  (Table 25)
 * ------------------------------------------------------------------ */
#define VSC8541_ANEXP_PAR_DETECT_FAULT  (1u <<  4)
#define VSC8541_ANEXP_LP_NP_ABLE        (1u <<  3)
#define VSC8541_ANEXP_NP_ABLE           (1u <<  2)
#define VSC8541_ANEXP_PAGE_RECEIVED     (1u <<  1)
#define VSC8541_ANEXP_LP_ANEG_ABLE      (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 7  –  Auto-Neg Next Page TX  (Table 26)
 * Reg 8  –  Auto-Neg LP Received Next Page  (same layout)
 * ------------------------------------------------------------------ */
#define VSC8541_ANNP_NEXT_PAGE          (1u << 15)
#define VSC8541_ANNP_MSG_PAGE           (1u << 13)
#define VSC8541_ANNP_ACK2               (1u << 12)
#define VSC8541_ANNP_TOGGLE             (1u << 11)
#define VSC8541_ANNP_MSG_MASK           (0x7FFu << 0)
#define VSC8541_ANNP_MSG_SHIFT          0u

/* ------------------------------------------------------------------
 * Reg 9  –  1000BASE-T Control  (Table 30)
 * ------------------------------------------------------------------ */
#define VSC8541_1GCTRL_TEST_MODE_MASK   (0x7u << 13)   /* bits[15:13]: 3-bit test mode */
#define VSC8541_1GCTRL_TEST_MODE_SHIFT  13u
#define VSC8541_1GCTRL_MS_MAN_CFG_EN   (1u << 12)      /* master/slave manual config enable */
#define VSC8541_1GCTRL_MS_CFG_MASTER   (1u << 11)      /* 1=master, 0=slave */
#define VSC8541_1GCTRL_PORT_MULTIPORT   (1u << 10)     /* 1=multiport device */
#define VSC8541_1GCTRL_1000T_FDX_ADV   (1u <<  9)      /* advertise 1000BASE-T FDX */
#define VSC8541_1GCTRL_1000T_HDX_ADV   (1u <<  8)      /* advertise 1000BASE-T HDX */

/* ------------------------------------------------------------------
 * Reg 10  –  1000BASE-T Status  (Table 31)
 * ------------------------------------------------------------------ */
#define VSC8541_1GSTS_MS_FAULT          (1u << 15)
#define VSC8541_1GSTS_MS_MASTER         (1u << 14)  /* 1=local is master */
#define VSC8541_1GSTS_LOCAL_RECV_OK     (1u << 13)
#define VSC8541_1GSTS_REMOTE_RECV_OK    (1u << 12)
#define VSC8541_1GSTS_LP_1G_FDX_CAP    (1u << 11)
#define VSC8541_1GSTS_LP_1G_HDX_CAP    (1u << 10)
#define VSC8541_1GSTS_IDLE_ERR_MASK     (0xFFu << 0)
#define VSC8541_1GSTS_IDLE_ERR_SHIFT    0u

/* ------------------------------------------------------------------
 * Reg 13  –  MMD Access Control  (Table 32)
 * ------------------------------------------------------------------ */
#define VSC8541_MMDCTRL_FN_MASK         (0x3u << 14)
#define VSC8541_MMDCTRL_FN_SHIFT        14u
#define VSC8541_MMDCTRL_FN_ADDR         0x0u
#define VSC8541_MMDCTRL_FN_DATA_NOINC   0x1u
#define VSC8541_MMDCTRL_FN_DATA_INC_RW  0x2u
#define VSC8541_MMDCTRL_FN_DATA_INC_W   0x3u
#define VSC8541_MMDCTRL_DEVAD_MASK      (0x1Fu << 0)
#define VSC8541_MMDCTRL_DEVAD_SHIFT     0u

/* ------------------------------------------------------------------
 * Reg 15  –  1000BASE-T Status Extension 1  (Table 34)
 * bit[15] reserved; bit[14]=1000BASE-X HDX; bit[13]=1000T FDX; bit[12]=1000T HDX
 * ------------------------------------------------------------------ */
#define VSC8541_EXTSTS_1000X_HDX_CAP    (1u << 14)
#define VSC8541_EXTSTS_1000T_FDX_CAP    (1u << 13)
#define VSC8541_EXTSTS_1000T_HDX_CAP    (1u << 12)

/* ------------------------------------------------------------------
 * Reg 16  –  100BASE-TX Status Extension  (Table 35, RO/SC)
 * ------------------------------------------------------------------ */
#define VSC8541_100TX_STAT_DESCRAMBLER  (1u << 15)  /* descrambler locked */
#define VSC8541_100TX_STAT_LOCK_ERR     (1u << 14)  /* SC */
#define VSC8541_100TX_STAT_DISCONNECT   (1u << 13)  /* SC */
#define VSC8541_100TX_STAT_LINK         (1u << 12)  /* link active */
#define VSC8541_100TX_STAT_RX_ERR       (1u << 11)  /* SC */
#define VSC8541_100TX_STAT_TX_ERR       (1u << 10)  /* SC */
#define VSC8541_100TX_STAT_SSD_ERR      (1u <<  9)  /* SC */
#define VSC8541_100TX_STAT_ESD_ERR      (1u <<  8)  /* SC */

/* ------------------------------------------------------------------
 * Reg 17  –  1000BASE-T Status Extension 2  (Table 36, RO/SC)
 * ------------------------------------------------------------------ */
#define VSC8541_1000T_STAT2_DESCRAMBLER (1u << 15)
#define VSC8541_1000T_STAT2_LOCK_ERR    (1u << 14)  /* SC */
#define VSC8541_1000T_STAT2_DISCONNECT  (1u << 13)  /* SC */
#define VSC8541_1000T_STAT2_LINK        (1u << 12)
#define VSC8541_1000T_STAT2_RX_ERR     (1u << 11)  /* SC */
#define VSC8541_1000T_STAT2_TX_ERR     (1u << 10)  /* SC */
#define VSC8541_1000T_STAT2_SSD_ERR    (1u <<  9)  /* SC */
#define VSC8541_1000T_STAT2_ESD_ERR    (1u <<  8)  /* SC */
#define VSC8541_1000T_STAT2_CAR_EXT    (1u <<  7)  /* carrier extension error, SC */
#define VSC8541_1000T_STAT2_BCM5400    (1u <<  6)  /* BCM5400 detected */
#define VSC8541_1000T_STAT2_MDI_XO_ERR (1u <<  5)  /* MDI crossover error, SC */

/* ------------------------------------------------------------------
 * Reg 18  –  Bypass Control  (Table 37)
 * ------------------------------------------------------------------ */
#define VSC8541_BYPASS_TX_DIS           (1u << 15)
#define VSC8541_BYPASS_4B5B             (1u << 14)
#define VSC8541_BYPASS_SCRAMBLER        (1u << 13)
#define VSC8541_BYPASS_DESCRAMBLER      (1u << 12)
#define VSC8541_BYPASS_PCS_RX           (1u << 11)
#define VSC8541_BYPASS_PCS_TX           (1u << 10)
#define VSC8541_BYPASS_LFI_TIMER        (1u <<  9)
/* bit 8 reserved */
#define VSC8541_BYPASS_HP_AUTOMDIX_DIS  (1u <<  7)  /* sticky, def=1 */
#define VSC8541_BYPASS_BCM5400_DET_DIS  (1u <<  6)  /* sticky */
#define VSC8541_BYPASS_PAIR_SWAP_DIS    (1u <<  5)  /* sticky */
#define VSC8541_BYPASS_POL_CORR_DIS     (1u <<  4)  /* sticky */
#define VSC8541_BYPASS_PAR_DET_CTRL     (1u <<  3)  /* sticky, def=1 */
#define VSC8541_BYPASS_PULSE_SHAPE_DIS  (1u <<  2)
#define VSC8541_BYPASS_1G_NP_DIS        (1u <<  1)  /* sticky */

/* ------------------------------------------------------------------
 * Reg 19  –  Error Counter 1  (Table 38, 8-bit, SC on read)
 * bits[7:0] = 100/1000 RX error counter; saturates at 255
 * ------------------------------------------------------------------ */
#define VSC8541_ERR_CNT1_MASK           0x00FFu

/* ------------------------------------------------------------------
 * Reg 20  –  Error Counter 2  (Table 39, 8-bit, SC on read)
 * bits[7:0] = 100/1000 false carrier counter
 * ------------------------------------------------------------------ */
#define VSC8541_ERR_CNT2_MASK           0x00FFu

/* ------------------------------------------------------------------
 * Reg 21  –  Error Counter 3  (Table 40, 8-bit, SC on read)
 * bits[7:0] = copper media link disconnect counter
 * ------------------------------------------------------------------ */
#define VSC8541_ERR_CNT3_MASK           0x00FFu

/* ------------------------------------------------------------------
 * Reg 22  –  Extended Control and Status  (Table 41)
 * ------------------------------------------------------------------ */
#define VSC8541_ECS_FORCE_10T_LINK      (1u << 15)  /* sticky */
#define VSC8541_ECS_JABBER_DIS          (1u << 14)  /* sticky */
#define VSC8541_ECS_10T_ECHO_DIS        (1u << 13)  /* sticky, def=1 */
#define VSC8541_ECS_SQE_DIS             (1u << 12)  /* sticky, def=1 */
#define VSC8541_ECS_SQUELCH_MASK        (0x3u << 10) /* sticky, def=00 */
#define VSC8541_ECS_SQUELCH_SHIFT       10u
#define VSC8541_ECS_STICKY_RST_EN       (1u <<  9)  /* super-sticky, def=1 */
#define VSC8541_ECS_EOF_ERR             (1u <<  8)  /* RO, SC */
#define VSC8541_ECS_10T_DISC_STATE      (1u <<  7)  /* RO, SC */
#define VSC8541_ECS_10T_LINK_STAT       (1u <<  6)  /* RO */
/* bits[5:1] reserved */
#define VSC8541_ECS_SMI_BCAST_WR        (1u <<  0)  /* sticky */

/* ------------------------------------------------------------------
 * Reg 23  –  Extended PHY Control 1  (Table 42)
 * ------------------------------------------------------------------ */
/* bits[15:14] reserved */
#define VSC8541_EPHYC1_MAC_CLK_EN       (1u << 13)  /* MAC supplied clock enable */
#define VSC8541_EPHYC1_MAC_IF_MASK      (0x3u << 11)
#define VSC8541_EPHYC1_MAC_IF_SHIFT     11u
#define VSC8541_EPHYC1_MAC_IF_GMII      (0x0u)      /* 00 = GMII/MII */
#define VSC8541_EPHYC1_MAC_IF_RMII      (0x1u)      /* 01 = RMII */
#define VSC8541_EPHYC1_MAC_IF_RGMII     (0x2u)      /* 10 = RGMII (default) */
/* bits[10:4] reserved */
#define VSC8541_EPHYC1_FAR_LOOPBACK     (1u <<  3)  /* far-end loopback enable */
/* bits[2:0] reserved */

/* ------------------------------------------------------------------
 * Reg 24  –  Extended PHY Control 2  (Table 43)
 * ------------------------------------------------------------------ */
#define VSC8541_EPHYC2_EDGE_RATE_MASK   (0x7u << 13) /* 100TX edge rate, sticky */
#define VSC8541_EPHYC2_EDGE_RATE_SHIFT  13u
#define VSC8541_EPHYC2_PICMG_PWR        (1u << 12)  /* sticky */
/* bits[11:6] reserved */
#define VSC8541_EPHYC2_JUMBO_MASK       (0x3u <<  4) /* jumbo packet mode, sticky */
#define VSC8541_EPHYC2_JUMBO_SHIFT      4u
/* bits[3:1] reserved */
#define VSC8541_EPHYC2_1000T_CONN_LB    (1u <<  0)  /* 1000T connector loopback */

/* ------------------------------------------------------------------
 * Reg 25  –  Interrupt Mask  (Table 44, sticky)
 * ------------------------------------------------------------------ */
#define VSC8541_INTM_MDINT_EN           (1u << 15)
#define VSC8541_INTM_SPEED_CHG          (1u << 14)
#define VSC8541_INTM_LINK_CHG           (1u << 13)
#define VSC8541_INTM_FDX_CHG            (1u << 12)
#define VSC8541_INTM_ANEG_ERR           (1u << 11)
#define VSC8541_INTM_ANEG_OK            (1u << 10)
#define VSC8541_INTM_POE_DET            (1u <<  9)
#define VSC8541_INTM_SYM_ERR           (1u <<  8)
#define VSC8541_INTM_FLF                (1u <<  7)
#define VSC8541_INTM_WOL                (1u <<  6)
#define VSC8541_INTM_EXT_INT            (1u <<  5)
/* bit 4 reserved */
#define VSC8541_INTM_FALSE_CAR          (1u <<  3)
#define VSC8541_INTM_DOWNSHIFT          (1u <<  2)
#define VSC8541_INTM_MS_ERR             (1u <<  1)
#define VSC8541_INTM_RX_ER              (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 26  –  Interrupt Status  (Table 45, SC on read)
 * ------------------------------------------------------------------ */
#define VSC8541_INT_ACTIVE              (1u << 15)  /* interrupt pending */
#define VSC8541_INT_SPEED_CHG           (1u << 14)
#define VSC8541_INT_LINK_CHG            (1u << 13)
#define VSC8541_INT_FDX_CHG             (1u << 12)
#define VSC8541_INT_ANEG_ERR            (1u << 11)
#define VSC8541_INT_ANEG_OK             (1u << 10)
#define VSC8541_INT_POE_DET             (1u <<  9)
#define VSC8541_INT_SYM_ERR             (1u <<  8)
#define VSC8541_INT_FLF                 (1u <<  7)
#define VSC8541_INT_WOL                 (1u <<  6)
#define VSC8541_INT_EXT_INT             (1u <<  5)
/* bit 4 reserved */
#define VSC8541_INT_FALSE_CAR           (1u <<  3)
#define VSC8541_INT_DOWNSHIFT           (1u <<  2)
#define VSC8541_INT_MS_ERR              (1u <<  1)
#define VSC8541_INT_RX_ER               (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 28  –  Auxiliary Control and Status  (Table 46)
 * ActiPHY timeout uses non-contiguous bits: {bit7=MSB, bit2=LSB}
 *   00=2.3s  01=3.3s  10=4.3s  11=5.3s
 * ------------------------------------------------------------------ */
#define VSC8541_ACS_LINK_UP             (1u << 15)  /* autoneg complete / link status */
#define VSC8541_ACS_ANEG_DIS            (1u << 14)
#define VSC8541_ACS_HP_MDIX_XOVER      (1u << 13)  /* HP AutoMDIX crossover indicator */
#define VSC8541_ACS_CD_PAIR_SWAP        (1u << 12)
#define VSC8541_ACS_A_POL               (1u << 11)  /* pair A polarity */
#define VSC8541_ACS_B_POL               (1u << 10)
#define VSC8541_ACS_C_POL               (1u <<  9)
#define VSC8541_ACS_D_POL               (1u <<  8)
#define VSC8541_ACS_ACTIPHY_TO_MSB      (1u <<  7)  /* ActiPHY timeout MSB, sticky */
#define VSC8541_ACS_ACTIPHY_EN          (1u <<  6)  /* ActiPHY mode enable, sticky */
#define VSC8541_ACS_FDX_STAT            (1u <<  5)
#define VSC8541_ACS_SPEED_MASK          (0x3u << 3)
#define VSC8541_ACS_SPEED_SHIFT         3u
#define VSC8541_ACS_SPEED_10T           (0x0u)
#define VSC8541_ACS_SPEED_100TX         (0x1u)
#define VSC8541_ACS_SPEED_1000T         (0x2u)
#define VSC8541_ACS_ACTIPHY_TO_LSB      (1u <<  2)  /* ActiPHY timeout LSB, sticky, def=1 */
#define VSC8541_ACS_MEDIA_MASK          (0x3u << 0)
#define VSC8541_ACS_MEDIA_NONE          (0x0u)
#define VSC8541_ACS_MEDIA_COPPER        (0x1u)

/* ------------------------------------------------------------------
 * Reg 29  –  LED Mode Select  (Table 47, sticky)
 * ------------------------------------------------------------------ */
#define VSC8541_LED_MODE_LED1_MASK      (0xFu << 4)  /* def=0101 */
#define VSC8541_LED_MODE_LED1_SHIFT     4u
#define VSC8541_LED_MODE_LED0_MASK      (0xFu << 0)  /* def=0100 */
#define VSC8541_LED_MODE_LED0_SHIFT     0u

/* ------------------------------------------------------------------
 * Reg 30  –  LED Behavior  (Table 48, sticky)
 * ------------------------------------------------------------------ */
/* bits[15:13] reserved */
#define VSC8541_LEDB_PULSING_EN         (1u << 12)
#define VSC8541_LEDB_BLINK_RATE_MASK    (0x3u << 10)  /* def=01=5Hz/200ms */
#define VSC8541_LEDB_BLINK_RATE_SHIFT   10u
/* bits[9:7] reserved */
#define VSC8541_LEDB_LED1_STRETCH       (1u <<  6)
#define VSC8541_LEDB_LED0_STRETCH       (1u <<  5)
/* bits[4:2] reserved */
#define VSC8541_LEDB_LED1_COMB_DIS      (1u <<  1)
#define VSC8541_LEDB_LED0_COMB_DIS      (1u <<  0)

/* ================================================================== */
/* Section 4.3  –  Extended Page 1 Registers  (page 0x0001)          */
/* ================================================================== */

/* Register addresses */
#define VSC8541_REG_E1_CRC_GOOD_CNT     18u  /* Table 51: Cu Media CRC Good Counter */
#define VSC8541_REG_E1_EXT_MODE_CTRL    19u  /* Table 52: Extended Mode Control */
#define VSC8541_REG_E1_EXT_PHY_CTRL3   20u  /* Table 53: Extended PHY Control 3 (ActiPHY) */
/* 21, 22 reserved */
#define VSC8541_REG_E1_EXT_PHY_CTRL4   23u  /* Table 54: Extended PHY Control 4 (PoE/CRC) */
/* 24-28 reserved */
#define VSC8541_REG_E1_EPG_CTRL1        29u  /* Table 55: EPG Control 1 */
#define VSC8541_REG_E1_EPG_CTRL2        30u  /* Table 56: EPG Control 2 */

/* ------------------------------------------------------------------
 * Reg 18 E1  –  Cu Media CRC Good Counter  (Table 51)
 * ------------------------------------------------------------------ */
#define VSC8541_E1_CRC_GOOD_PKT         (1u << 15)   /* packet received since last read, SC */
/* bit 14 reserved */
#define VSC8541_E1_CRC_GOOD_CNT_MASK    0x3FFFu      /* CRC good count mod 10000, SC */

/* ------------------------------------------------------------------
 * Reg 19 E1  –  Extended Mode Control  (Table 52, sticky)
 * ------------------------------------------------------------------ */
/* bits[15:14] reserved */
#define VSC8541_E1_EMC_LED1_EXT         (1u << 13)
#define VSC8541_E1_EMC_LED0_EXT         (1u << 12)
#define VSC8541_E1_EMC_LED_BLINK_SUPP   (1u << 11)
/* bits[10:5] reserved */
#define VSC8541_E1_EMC_FAST_LINK        (1u <<  4)   /* fast link failure, def=1 */
#define VSC8541_E1_EMC_MDI_XO_MASK      (0x3u << 2)  /* 00=HP AutoMDIX, 10=MDI, 11=MDI-X */
#define VSC8541_E1_EMC_MDI_XO_SHIFT     2u
/* bits[1:0] reserved */

/* ------------------------------------------------------------------
 * Reg 20 E1  –  Extended PHY Control 3 / ActiPHY  (Table 53)
 * ------------------------------------------------------------------ */
#define VSC8541_E1_EPHYC3_CAR_EXT_DIS   (1u << 15)  /* disable carrier extension, def=1 */
#define VSC8541_E1_EPHYC3_SLEEP_TIMER   (0x3u << 13) /* sticky: 00=1s,01=2s,10=3s,11=4s */
#define VSC8541_E1_EPHYC3_SLEEP_SHIFT   13u
#define VSC8541_E1_EPHYC3_WAKEUP_TIMER  (0x3u << 11) /* sticky: 00=160ms,01=400ms,10=800ms,11=2s */
#define VSC8541_E1_EPHYC3_WAKEUP_SHIFT  11u
#define VSC8541_E1_EPHYC3_SLOW_MDC      (1u << 10)  /* sticky */
/* bits[9:8] reserved */
#define VSC8541_E1_EPHYC3_MEDIA_MODE    (0x3u <<  6) /* RO */
#define VSC8541_E1_EPHYC3_MEDIA_SHIFT   6u
#define VSC8541_E1_EPHYC3_10T_NOPREAMB  (1u <<  5)  /* sticky */
#define VSC8541_E1_EPHYC3_DSHIFT_EN     (1u <<  4)  /* link speed autodownshift enable, sticky */
#define VSC8541_E1_EPHYC3_DSHIFT_CTRL   (0x3u <<  2) /* sticky, def=01 */
#define VSC8541_E1_EPHYC3_DSHIFT_SHIFT  2u
#define VSC8541_E1_EPHYC3_DSHIFT_STAT   (1u <<  1)  /* downshift status, RO */

/* ------------------------------------------------------------------
 * Reg 23 E1  –  Extended PHY Control 4 / PoE  (Table 54)
 * ------------------------------------------------------------------ */
#define VSC8541_E1_EPHYC4_PHYADDR_MASK  (0x1Fu << 11)  /* PHY address, RO */
#define VSC8541_E1_EPHYC4_PHYADDR_SHIFT 11u
#define VSC8541_E1_EPHYC4_INLINE_PWR_EN (1u << 10)     /* inline power detect enable, sticky */
#define VSC8541_E1_EPHYC4_INLINE_STAT   (0x3u <<  8)  /* inline power detect status, RO */
#define VSC8541_E1_EPHYC4_INLINE_SHIFT  8u
#define VSC8541_E1_EPHYC4_CRC_ERR_MASK  0x00FFu       /* Cu media CRC error counter, RO SC */

/* ------------------------------------------------------------------
 * Reg 29 E1  –  EPG Control 1  (Table 55)
 * ------------------------------------------------------------------ */
#define VSC8541_E1_EPG1_ENABLE          (1u << 15)
#define VSC8541_E1_EPG1_RUN             (1u << 14)
#define VSC8541_E1_EPG1_TX_DURATION     (1u << 13)
#define VSC8541_E1_EPG1_PKT_LEN_MASK    (0x3u << 11)
#define VSC8541_E1_EPG1_PKT_LEN_SHIFT   11u
#define VSC8541_E1_EPG1_IPG_MASK        (1u << 10)
#define VSC8541_E1_EPG1_DEST_ADDR_MASK  (0xFu <<  6)
#define VSC8541_E1_EPG1_DEST_ADDR_SHIFT 6u
#define VSC8541_E1_EPG1_SRC_ADDR_MASK   (0xFu <<  2)
#define VSC8541_E1_EPG1_SRC_ADDR_SHIFT  2u
#define VSC8541_E1_EPG1_PAYLOAD_TYPE    (1u <<  1)
#define VSC8541_E1_EPG1_BAD_FCS         (1u <<  0)

/* Reg 30 E1  –  EPG Control 2  (Table 56): bits[15:0] = packet payload pattern */

/* ================================================================== */
/* Section 4.4  –  Extended Page 2 Registers  (page 0x0002)          */
/* ================================================================== */

/* Register addresses */
#define VSC8541_REG_E2_CU_PMD_TX_CTRL  16u  /* Table 58: Cu PMD Transmit Control */
#define VSC8541_REG_E2_EEE_CTRL        17u  /* Table 59: EEE Control */
/* 18, 19 reserved */
#define VSC8541_REG_E2_RGMII_CTRL      20u  /* Table 60: RGMII Control */
#define VSC8541_REG_E2_WOL_MAC0        21u  /* Table 61: WoL MAC Address [15:0] */
#define VSC8541_REG_E2_WOL_MAC1        22u  /* Table 62: WoL MAC Address [31:16] */
#define VSC8541_REG_E2_WOL_MAC2        23u  /* Table 63: WoL MAC Address [47:32] */
#define VSC8541_REG_E2_SECPWD0         24u  /* Table 64: Secure-On Password [15:0] */
#define VSC8541_REG_E2_SECPWD1         25u  /* Table 65: Secure-On Password [31:16] */
#define VSC8541_REG_E2_SECPWD2         26u  /* Table 66: Secure-On Password [47:32] */
#define VSC8541_REG_E2_WOL_MAC_CTRL    27u  /* Table 67: WoL and MAC Interface Control */
#define VSC8541_REG_E2_EXT_INT_MASK    28u  /* Table 68: Extended Interrupt Mask */
#define VSC8541_REG_E2_EXT_INT_STAT    29u  /* Table 69: Extended Interrupt Status – SC */
#define VSC8541_REG_E2_RING_RESIL      30u  /* Table 70: Ring Resiliency Control */

/* ------------------------------------------------------------------
 * Reg 16 E2  –  Cu PMD Transmit Control  (Table 58)
 * ------------------------------------------------------------------ */
#define VSC8541_E2_CPTC_1000T_TRIM_MASK (0xFu << 12)
#define VSC8541_E2_CPTC_1000T_TRIM_SHIFT 12u
#define VSC8541_E2_CPTC_100TX_TRIM_MASK (0xFu <<  8)
#define VSC8541_E2_CPTC_100TX_TRIM_SHIFT 8u
#define VSC8541_E2_CPTC_10T_TRIM_MASK   (0xFu <<  4)  /* def=1000 */
#define VSC8541_E2_CPTC_10T_TRIM_SHIFT  4u
#define VSC8541_E2_CPTC_10TE_TRIM_MASK  (0xFu <<  0)  /* def=1110 */
#define VSC8541_E2_CPTC_10TE_TRIM_SHIFT 0u

/* ------------------------------------------------------------------
 * Reg 17 E2  –  EEE Control  (Table 59)
 * ------------------------------------------------------------------ */
#define VSC8541_E2_EEE_10BASE_TE_EN     (1u << 15)
/* bits[14:12] reserved */
#define VSC8541_E2_EEE_LED_POL_MASK     (0x3u << 10)  /* invert LED polarity, sticky */
#define VSC8541_E2_EEE_LED_POL_SHIFT    10u
/* bit 9 reserved */
#define VSC8541_E2_EEE_LINK_STAT        (1u <<  8)   /* link status, RO */
#define VSC8541_E2_EEE_1000T_STAT       (1u <<  7)   /* 1000T EEE enabled, RO */
#define VSC8541_E2_EEE_100TX_STAT       (1u <<  6)   /* 100TX EEE enabled, RO */
#define VSC8541_E2_EEE_1000T_FORCE      (1u <<  5)   /* force 1000T EEE, sticky */
#define VSC8541_E2_EEE_FORCE_TX_LPI    (1u <<  4)   /* force TX LPI, sticky */
#define VSC8541_E2_EEE_100TX_TX_INH    (1u <<  3)   /* inhibit 100TX TX EEE LPI, sticky */
#define VSC8541_E2_EEE_100TX_RX_INH    (1u <<  2)   /* inhibit 100TX RX EEE LPI, sticky */
#define VSC8541_E2_EEE_1000T_TX_INH    (1u <<  1)   /* inhibit 1000T TX EEE LPI, sticky */
#define VSC8541_E2_EEE_1000T_RX_INH    (1u <<  0)   /* inhibit 1000T RX EEE LPI, sticky */

/* ------------------------------------------------------------------
 * Reg 20 E2  –  RGMII Control  (Table 60, sticky)
 * RX_CLK/TX_CLK delay: 000=0.2ns, 001=0.8ns, ... 111=3.4ns (step ~0.45ns)
 * ------------------------------------------------------------------ */
#define VSC8541_RGMII_FLF2_EN           (1u << 15)
/* bits[14:13] reserved */
#define VSC8541_RGMII_SOF_EN            (1u << 12)
/* bits[11:8] reserved */
#define VSC8541_RGMII_RXD_REV           (1u <<  7)   /* RGMII/GMII RXD bit reversal */
#define VSC8541_RGMII_RX_CLK_MASK       (0x7u << 4)  /* RX_CLK delay */
#define VSC8541_RGMII_RX_CLK_SHIFT      4u
#define VSC8541_RGMII_TXD_REV           (1u <<  3)   /* RGMII/GMII TXD bit reversal */
#define VSC8541_RGMII_TX_CLK_MASK       (0x7u << 0)  /* TX_CLK delay, same encoding */
#define VSC8541_RGMII_TX_CLK_SHIFT      0u
/* CLK delay field values – apply to both RX_CLK and TX_CLK fields */
#define VSC8541_RGMII_CLK_DLY_0_2NS    0x0u   /* 0.2 ns */
#define VSC8541_RGMII_CLK_DLY_0_8NS    0x1u   /* 0.8 ns */
#define VSC8541_RGMII_CLK_DLY_1_1NS    0x2u   /* 1.1 ns */
#define VSC8541_RGMII_CLK_DLY_1_7NS    0x3u   /* 1.7 ns */
#define VSC8541_RGMII_CLK_DLY_2_0NS    0x4u   /* 2.0 ns */
#define VSC8541_RGMII_CLK_DLY_2_3NS    0x5u   /* 2.3 ns */
#define VSC8541_RGMII_CLK_DLY_2_6NS    0x6u   /* 2.6 ns */
#define VSC8541_RGMII_CLK_DLY_3_4NS    0x7u   /* 3.4 ns */
/* Usage example – set RX delay 2.0 ns, TX delay 2.0 ns:
 *   val = VSC8541_FIELD_PREP(VSC8541_RGMII_CLK_DLY_2_0NS, VSC8541_RGMII_RX_CLK_MASK, VSC8541_RGMII_RX_CLK_SHIFT)
 *       | VSC8541_FIELD_PREP(VSC8541_RGMII_CLK_DLY_2_0NS, VSC8541_RGMII_TX_CLK_MASK, VSC8541_RGMII_TX_CLK_SHIFT);
 */

/* ------------------------------------------------------------------
 * Reg 27 E2  –  WoL and MAC Interface Control  (Table 67, sticky)
 * ------------------------------------------------------------------ */
#define VSC8541_E2_WMC_SECURE_ON_EN     (1u << 15)
#define VSC8541_E2_WMC_SECURE_ON_LEN    (1u << 14)  /* 0=4B, 1=6B password */
/* bits[13:12] reserved */
#define VSC8541_E2_WMC_ADDR_RPT_MASK    (0xFu <<  8)  /* address repetition count, def=1111 */
#define VSC8541_E2_WMC_ADDR_RPT_SHIFT   8u
#define VSC8541_E2_WMC_PAD_EDGE_MASK    (0x7u <<  5)  /* pad edge rate control, def=111 */
#define VSC8541_E2_WMC_PAD_EDGE_SHIFT   5u
#define VSC8541_E2_WMC_RMII_CLKOUT_EN   (1u <<  4)   /* RMII CLKOUT enable, def=1 */
/* bits[3:1] reserved */
#define VSC8541_E2_WMC_MDINT_CMOS       (1u <<  0)   /* MDINT CMOS drive */

/* ------------------------------------------------------------------
 * Reg 28 E2  –  Extended Interrupt Mask  (Table 68, sticky)
 * ------------------------------------------------------------------ */
/* bits[15:14] reserved */
#define VSC8541_E2_EIM_RX_FIFO_OVF      (1u << 13)
#define VSC8541_E2_EIM_TX_FIFO_OVF      (1u << 12)
/* bits[11:5] reserved */
#define VSC8541_E2_EIM_RR_SW_DONE       (1u <<  4)
#define VSC8541_E2_EIM_EEE_LINK_FAIL    (1u <<  3)
#define VSC8541_E2_EIM_EEE_RX_TQ        (1u <<  2)
#define VSC8541_E2_EIM_EEE_WAIT_QUIET   (1u <<  1)
#define VSC8541_E2_EIM_EEE_WAKE_ERR     (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 29 E2  –  Extended Interrupt Status  (Table 69, SC on read)
 * ------------------------------------------------------------------ */
/* bits[15:14] reserved */
#define VSC8541_E2_EIS_RX_FIFO_OVF      (1u << 13)
#define VSC8541_E2_EIS_TX_FIFO_OVF      (1u << 12)
/* bits[11:5] reserved */
#define VSC8541_E2_EIS_RR_SW_DONE       (1u <<  4)
#define VSC8541_E2_EIS_EEE_LINK_FAIL    (1u <<  3)
#define VSC8541_E2_EIS_EEE_RX_TQ        (1u <<  2)
#define VSC8541_E2_EIS_EEE_WAIT_QUIET   (1u <<  1)
#define VSC8541_E2_EIS_EEE_WAKE_ERR     (1u <<  0)

/* ------------------------------------------------------------------
 * Reg 30 E2  –  Ring Resiliency Control  (Table 70)
 * ------------------------------------------------------------------ */
#define VSC8541_E2_RRC_STARTUP_EN       (1u << 15)  /* sticky */
#define VSC8541_E2_RRC_ADVERTISE        (1u << 14)  /* sticky */
#define VSC8541_E2_RRC_LP_ADV           (1u << 13)  /* LP ring resiliency adv, RO */
#define VSC8541_E2_RRC_FORCE_EN         (1u << 12)  /* sticky */
/* bits[11:6] reserved */
#define VSC8541_E2_RRC_STATUS_MASK      (0x3u <<  4) /* ring resiliency status, RO */
#define VSC8541_E2_RRC_STATUS_SHIFT     4u
/* bits[3:1] reserved */
#define VSC8541_E2_RRC_START_SW         (1u <<  0)  /* start switchover, RWSC */

/* ================================================================== */
/* Helper macros                                                       */
/* ================================================================== */

#define VSC8541_FIELD_GET(val, mask, shift) (((val) & (mask)) >> (shift))
#define VSC8541_FIELD_PREP(x, mask, shift)  (((u16)(x) << (shift)) & (mask))

#endif /* VSC8541_REGS_H */
