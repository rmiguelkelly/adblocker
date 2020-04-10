
#include <iostream>
using std::string;

class dns_forwarder {
    private:
        bool is_running;
        int run_main_loop();
        unsigned int port;
        void handle_dns_request(int socket);
        void print_dns_packet(char packet[], int size);
        void send_bad_request(short dns_id, int socket, struct sockaddr *addr, unsigned int size);
    public:
        dns_forwarder();
        void Start();
        void Stop();
};