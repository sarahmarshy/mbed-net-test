#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE

NetworkInterface* get_net() {
    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;
    return net;
}

int net_connect(NetworkInterface* net) {
    return MBED_CONF_APP_CONNECT_STATEMENT;
}
