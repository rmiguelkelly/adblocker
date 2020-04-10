
#include "../include/adblocker/dns_server.h"
#include "../include/adblocker/dns_forwarder.h"

#include <iostream>
#include <map>
#include <fstream>

int main(int argc, char *args[]) {

    dns_forwarder server;
    server.Start();

    struct testpack {
        unsigned short val1;
        unsigned char val2;
        unsigned char val3;
    };

    struct testpack test1 = { 0xABCD, 0x2, 0x3 };

    char *buffer = (char*)&test1;

    struct testpack *test2 = (struct testpack*)buffer;

    printf("0x%X\n", test2->val1);
    printf("0x%X\n", test2->val2);
    printf("0x%X\n", test2->val3);


    return 0;
}