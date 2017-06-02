#define ETHERNET        1
#define WIFI_ESP8266    2
#define MESH_LOWPAN_ND  3
#define MESH_THREAD     4
#define WIFI_ODIN       5
#define CELL_C027       6

#if MBED_CONF_APP_NETWORK_INTERFACE == ETHERNET
#include "net-help-eth.h"
#endif 

#if MBED_CONF_APP_NETWORK_INTERFACE == CELL_C027
#include "net-help-c027.h"
#endif 
