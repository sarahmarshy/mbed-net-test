#include "mbed.h"
#include "C027Interface.h"

NetworkInterface* get_net() {
    C027Interface* net = new C027Interface();
    return net;
}

int net_connect(NetworkInterface* net) {
    return ((C027Interface *)net)->connect(MBED_CONF_APP_CELL_APN);
}
