#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <map>
#include <boost/thread/shared_mutex.hpp>

using namespace std;

class dns_entry{

};

class dns_cache
{
    map<string, dns_entry> entries;
    mutable boost::shared_mutex entry_mutex;
public:
    bool find_entry(string const& domain) const
    {
        boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
        map<string, dns_entry>::const_iterator  it = entries.find(domain);
        return (it == entries.end()) ? false : true;
    }
    void update_or_add_entry(string const& domain, dns_entry const& dns_details)
    {
        lock_guard<boost::shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
     
};


int main(int argc, const char** argv) {
    dns_cache cache;
    dns_entry entry;
    cache.update_or_add_entry("baidu", entry);
    if (cache.find_entry("baidu"))
        std::cout << "find baidu" << std::endl;
    else
        std::cout << "can't find baidu" << std::endl;

    if (cache.find_entry("google"))
        std::cout << "find google" << std::endl;
    else
        std::cout << "can't find google" << std::endl;

    cache.update_or_add_entry("google", entry);

    if (cache.find_entry("google"))
        std::cout << "find google" << std::endl;
    else
        std::cout << "can't find google" << std::endl;

    return 0;
}