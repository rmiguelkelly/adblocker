

#include "dns_server.h"
#include <iostream>
#ifdef __WIN32__
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

using std::cout;
using std::endl;

dns_server::dns_server(std::string &server, unsigned int port) {
    this->dns_auth_addr = server;
    this->port = port;
}

int dns_server::send_dns_packet_raw(const char send_bytes[], int send_size, char recv_buffer[], int recv_size) {

    int dns_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (dns_socket < 0) {
        cout << "Unable to create dns connection socket" << endl;
        return -1;
    }

    //set timeout for 5 seconds max for both send/recv
    struct timeval timeout;      
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if (setsockopt (dns_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        cout << "Unable to set recieve timeout" << endl;
        return -1;
    }

    if (setsockopt (dns_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        cout << "Unable to set sending timeout" << endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    inet_pton(AF_INET, this->dns_auth_addr.c_str(), (void*)&addr.sin_addr.s_addr);
    addr.sin_port = htons(this->port);
    addr.sin_family = AF_INET;

    int bytes_sent = sendto(dns_socket, send_bytes, send_size, 0, (struct sockaddr*)&addr, sizeof(addr));

    if (bytes_sent < 0) {
        cout << "Client was unable to send request to that address" << endl;
        return -1;
    }

    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof(storage));
    socklen_t store_size = sizeof(storage);

    int bytes_recv = recvfrom(dns_socket, recv_buffer, recv_size, 0, NULL, NULL);

    if (bytes_recv < 0) {
        cout << "Client was unable to recieve request" << endl;
        return -1;
    }

    close(dns_socket);

    return bytes_recv;
}