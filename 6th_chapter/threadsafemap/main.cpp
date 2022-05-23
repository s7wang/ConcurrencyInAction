#include <iostream>
#include <string>
#include "threadsafe_lookup_table.h"

int main(int argc, char const *argv[])
{
    threadsafe_lookup_table<std::string , int> table;
    table.add_or_update_mapping("a", 1);
    table.add_or_update_mapping("b", 2);
    table.add_or_update_mapping("c", 3);

    std::cout << table.Value_for("b") << std::endl;
    
    return 0;
}
