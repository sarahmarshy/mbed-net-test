#include "mbed.h"
#include "EthernetInterface.h"

NetworkInterface* get_net() {
    EthernetInterface* net = new EthernetInterface();
    return net;
}

int net_connect(NetworkInterface* net) {
    return net->connect();
}
