#pragma once

#include <vector>
#include <string>
#include <ArduinoJson.h>
#undef B1

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
    
    static void add(Brew brew);

private:
    static JsonDocument httpGet(const std::string& url);
};
