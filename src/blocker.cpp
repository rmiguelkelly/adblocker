
#include "../include/adblocker/dns_forwarder.h"

#include <iostream>
#include <vector>

int main(int argc, char *args[]) {

    dns_forwarder server;
    server.Start();

    return 0;
}