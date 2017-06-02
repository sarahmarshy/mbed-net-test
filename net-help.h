#define ETHERNET        1
#define WIFI_ESP8266    2
#define MESH_LOWPAN_ND  3
#define MESH_THREAD     4
#define WIFI_ODIN       5

#if MBED_CONF_APP_NETWORK_INTERFACE == ETHERNET
#include "net-help-eth.h"
#endif 
