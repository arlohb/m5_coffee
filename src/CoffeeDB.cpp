#include "CoffeeDB.h"

std::vector<std::string> CoffeeDB::getCoffees() {
    return { "1", "2", "3" };
}

std::vector<Brew> CoffeeDB::get(const std::string& coffee) {
    return {
        { 0, coffee, 10.0f, 2.5f, 25.0f, 30.0f, (float)(20 + rand() % 10) },
        { 1, coffee, 15.0f, 2.5f, 37.5f, 42.0f, (float)(20 + rand() % 10) },
        { 2, coffee, 20.0f, 2.5f, 50.0f, 53.0f, (float)(20 + rand() % 10) }
    };
}
