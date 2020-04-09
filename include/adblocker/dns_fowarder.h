
#include <iostream>
using std::string;

class dns_fowarder {
    private:
        dns_fowarder(int port);
        dns_fowarder(std::string, int port);
        bool is_running;
        int run_main_loop();
    public:
        void Start();
        void Stop();
};