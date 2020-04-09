
#include "../include/adblocker/dns_server.h"

#include <iostream>
using std::string;

int main(int argc, char *args[]) {

    std::cout << "Begin send request" << std::endl;

    string server_addr = "1.1.1.1";
    dns_server request(server_addr, 53);
    
    const char request_buffer[] = { 0xF0, 0x1F, 0x43 };
    char buffer[1024];

    int s = request.send_dns_packet_raw(request_buffer, sizeof(request_buffer), buffer, sizeof(buffer));

    std::cout << s << std::endl;

    return 0;
}