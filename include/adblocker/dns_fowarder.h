
#include <iostream>
using std::string;

class dns_fowarder {
    private:
        bool is_running;
        int run_main_loop();
        unsigned int port;
        void handle_dns_request(int socket);
    public:
        dns_fowarder();
        void Start();
        void Stop();
};