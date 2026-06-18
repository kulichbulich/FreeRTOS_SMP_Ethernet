/*
 * net_udp.h - simple UDP echo server running on CPU0 (FreeRTOS SMP + lwIP RAW).
 *
 * lwIP is configured in the BSP in RAW/callback mode (NO_SYS=1). All lwIP work
 * (init, xemacif_input poll, UDP callback) happens in a SINGLE task pinned by
 * affinity to CPU0 - lwIP RAW is not thread-safe.
 */

#ifndef NET_UDP_H_
#define NET_UDP_H_

/*
 * Creates the network task pinned to CPU0. Call from main() before vTaskStartScheduler().
 * The task itself performs the PHY reset, EMAC/lwIP init, IP assignment and starts
 * the UDP echo server.
 */
void vStartNetworkTask( void );

#endif /* NET_UDP_H_ */
