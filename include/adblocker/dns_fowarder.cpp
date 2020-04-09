

#include "dns_fowarder.h"

void dns_fowarder::Start() {
    this->is_running = true;
    this->run_main_loop();
}

void dns_fowarder::Stop() {
    this->is_running = false;
}

int dns_fowarder::run_main_loop() {
    
    while (is_running) {

    }
}