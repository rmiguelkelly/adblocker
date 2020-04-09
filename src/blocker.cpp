
#include "../include/adblocker/dns_server.h"
#include "../include/adblocker/dns_fowarder.h"

#include <iostream>
using std::string;

int main(int argc, char *args[]) {

    dns_fowarder server;
    server.Start();
    return 0;
}