
#include "dns_fowarder.h"
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

dns_fowarder::dns_fowarder() {
    this->port = 53;

}

void dns_fowarder::Start() {
    this->is_running = true;
    this->run_main_loop();
}

void dns_fowarder::Stop() {
    this->is_running = false;
}

int dns_fowarder::run_main_loop() {
    
    int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (listener < 0) {
        return 0;
    }

    int y = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)) < 0) {
        cout << "Unable to set reuse address to true" << endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_port = htons(53);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cout << "Unable to bind socket" << endl;
        return -1;
    }

    cout << "Awaiting DNS requests..." << endl;
    while (is_running) {
        this->handle_dns_request(listener);
    }

    return 0;
}

void dns_fowarder::handle_dns_request(int socket) {

    struct sockaddr_storage store;
    memset(&store, 0, sizeof(store));
    socklen_t store_size = sizeof(store);

    char buffer[1024];
    int recv_size = recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&store, &store_size);
    
    char to_client_buffer[1024];
    
    std::string server_addr = "8.8.8.8";
    unsigned int port = 53;
    dns_server server(server_addr, port);

    cout << "Forwarding request" << endl;
    int recv_size_auth = server.send_dns_packet_raw(buffer, recv_size, to_client_buffer, sizeof(to_client_buffer));

    int sent = sendto(socket, to_client_buffer, recv_size_auth, 0, (struct sockaddr*)&store, store_size);

    cout << sent << endl;
}