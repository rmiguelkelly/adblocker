
#include "domain_blacklist.h"
#include <iostream>
#include <fstream>

domain_blacklist::domain_blacklist(std::string &path) {
    std::ifstream reader(path);
    std::string line;
    while (std::getline(reader, line)) {
        this->blacklisted_domains.push_back(line);
    }
    //Sort the array of domains in order to have a O(logn) time complexity when searching
    std::sort(this->blacklisted_domains.begin(), this->blacklisted_domains.end());
}

bool domain_blacklist::should_block(std::string &domain) {
    return std::binary_search(this->blacklisted_domains.begin(), this->blacklisted_domains.end(), domain);
}

int domain_blacklist::get_blacklist_count() {
    return this->blacklisted_domains.size();
}