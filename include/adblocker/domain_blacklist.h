
#include <iostream>
#include <vector>

class domain_blacklist {
    public:
        domain_blacklist(std::string &path);
        bool should_block(std::string &domain);
        int get_blacklist_count();
    private:
        std::vector<std::string> blacklisted_domains;
};