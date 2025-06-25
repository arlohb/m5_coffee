#pragma once

#include <vector>
#include <string>

struct Brew {
    unsigned long timestamp;
    std::string coffee;
    float in;
    float ratio;
    float aimOut;
    float actualOut;
    float brewTime;
};

class CoffeeDB {
public:
    CoffeeDB() = delete;
    
    static std::vector<std::string> getCoffees();
    
    static std::vector<Brew> get(const std::string& coffee);
};
