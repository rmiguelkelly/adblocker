
#include <iostream>
using std::string;

class dns_forwarder {
    private:
        bool is_running;
        int run_main_loop();
        unsigned int port;
        void handle_dns_request(int socket);
    public:
        dns_forwarder();
        void Start();
        void Stop();
};