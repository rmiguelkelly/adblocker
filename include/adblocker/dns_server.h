
#include <iostream>

class dns_server {
    public:
        dns_server(std::string &server, unsigned int port);        /*
        Synchronous operation to send raw buffer to a specified DNS server such as 8.8.8.8 or 1.1.1.1
        */
        int send_dns_packet_raw(const char send_bytes[], int send_size, char recv_buffer[], int recv_size);
    private:
        unsigned int port;
        std::string dns_auth_addr;
};