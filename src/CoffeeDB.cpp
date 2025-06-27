#include "CoffeeDB.h"

#include <HTTPClient.h>
#include "Secrets.h"
#include "Utils.h"

std::vector<std::string> CoffeeDB::getCoffees() {
    JsonDocument doc = httpGet(std::string(secrets::coffeeDbEndpoint) + "/get-coffees");
    std::vector<std::string> coffees;

    for (JsonVariant coffee : doc.as<JsonArray>()) {
        coffees.push_back(coffee.as<std::string>());
    }

    return coffees;
}

std::vector<Brew> CoffeeDB::get(const std::string& coffee) {
    delay(2000);
    return {
        { 0, coffee, 10.0f, 2.5f, 25.0f, 30.0f, (float)(20 + rand() % 10) },
        { 1, coffee, 15.0f, 2.5f, 37.5f, 42.0f, (float)(20 + rand() % 10) },
        { 2, coffee, 20.0f, 2.5f, 50.0f, 53.0f, (float)(20 + rand() % 10) }
    };
}

JsonDocument CoffeeDB::httpGet(const std::string& url) {
    HTTPClient http;
    http.begin(url.c_str());

    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        dbgln("HTTP GET failed with code: {}", code);
        http.end();
        return JsonDocument();
    }
    
    JsonDocument doc;
    deserializeJson(doc, http.getString());

    http.end();
    return doc;
}
