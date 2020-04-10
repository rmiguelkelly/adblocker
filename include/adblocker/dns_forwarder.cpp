
#include "dns_forwarder.h"
#include "dns_server.h"
#include "dns_packet.h"
#include <iostream>
#include <thread>

#ifdef __WIN32__
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#endif

using std::cout;
using std::endl;

dns_forwarder::dns_forwarder() {
    this->port = 53;
    std::string path = "./blacklist.txt";
    this->blacklist = std::make_unique<domain_blacklist>(path);
}

void dns_forwarder::Start() {
    this->is_running = true;
    
    this->run_main_loop();
}

void dns_forwarder::Stop() {
    this->is_running = false;
}

int dns_forwarder::run_main_loop() {
    
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

    this->print_server_details();

    while (is_running) {
        this->handle_dns_request(listener);
    }

    cout << "DNS server has stopped running" << endl;
    return 0;
}

void dns_forwarder::handle_dns_request(int socket) {

    bool should_drop = false;

    struct sockaddr_storage store;
    memset(&store, 0, sizeof(store));
    socklen_t store_size = sizeof(store);

    char buffer[1024];
    int recv_size = recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&store, &store_size);

    struct dns_packet *recv_packet = (struct dns_packet*)buffer;

    std::string url = "";
    int start_index = sizeof(struct dns_packet);

    while (buffer[start_index] != 0) {
        int len = buffer[start_index];
        for (int i = start_index + 1; i <= start_index + len; i++) {
            url += buffer[i];
        }
        url += '.';
        start_index += len + 1;
    }
    url.pop_back();

    if (this->should_block(url)) {
        //If in debug mode, print out which domains have been blocked
        if (BLOCKER_DEBUG) {
            cout << "DROPPED FOR " << url << endl;
        }
        send_bad_request(recv_packet->id, socket, (struct sockaddr*)&store, store_size);
        return;
    }
    
    char to_client_buffer[1024];
    
    std::string server_addr = "8.8.8.8";
    unsigned int port = 53;
    dns_server server(server_addr, port);

    int recv_size_auth = server.send_dns_packet_raw(buffer, recv_size, to_client_buffer, sizeof(to_client_buffer));

    int sent = sendto(socket, to_client_buffer, recv_size_auth, 0, (struct sockaddr*)&store, store_size);
}

void dns_forwarder::send_bad_request(short dns_id, int socket, struct sockaddr *addr, socklen_t size) {
    dns_packet bad_packet;
    memset(&bad_packet, 0, sizeof(bad_packet));
    bad_packet.id = dns_id;
    bad_packet.rcode    = 0x3;
    bad_packet.qr       = 0x1;
    bad_packet.opcode   = 0x0;

    char *buffer = (char*)&bad_packet;

    sendto(socket, buffer, sizeof(bad_packet), 0, addr, size);
}

bool dns_forwarder::should_block(string url) {
    return this->blacklist->should_block(url);
}

void dns_forwarder::print_server_details() {
    char hostname_buffer[256];
    int hname_len = gethostname(hostname_buffer, sizeof(hostname_buffer));
    if (hname_len < 0) {
        return;
    }

    cout << "========================================" << endl;
    cout << "| Home DNS server awaiting clients..." << endl;
    cout << "| Host name: " << hostname_buffer << endl;

    struct ifaddrs *iflist, *iface;
    if (getifaddrs(&iflist) < 0) {
        return;
    }
    for (iface = iflist; iface; iface = iface->ifa_next) {
        int af = iface->ifa_addr->sa_family;
        const void *addr;
        char addrp[INET6_ADDRSTRLEN];
        switch (af) {
            case AF_INET:
                addr = &((struct sockaddr_in *)iface->ifa_addr)->sin_addr;
                break;
            default:
                addr = NULL;
        }
        if (addr) {
            if (inet_ntop(af, addr, addrp, sizeof addrp) == NULL) {
                continue;
            }
            cout << "| Available IPv4 Address: " << addrp << endl;
        }
    }
    freeifaddrs(iflist);
    cout << "| Process ID: " << getpid() << endl;
    cout << "========================================" << endl;    
}