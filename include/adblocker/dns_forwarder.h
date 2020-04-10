
#include <iostream>
#include "domain_blacklist.h"

#define BLOCKER_DEBUG 1

using std::string;

class dns_forwarder {
    private:
        bool is_running;
        std::unique_ptr<domain_blacklist> blacklist;

        int run_main_loop();
        unsigned int port;
        void handle_dns_request(int socket);
        void send_bad_request(short dns_id, int socket, struct sockaddr *addr, unsigned int size);
        bool should_block(string url);
        void print_server_details();
    public:
        dns_forwarder();
        void Start();
        void Stop();
};