#include "CoffeeDB.h"

#include <HTTPClient.h>
#include <UrlEncode.h>
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
    JsonDocument doc = httpGet(std::string(secrets::coffeeDbEndpoint) + "/get?filter=" + urlEncode(coffee.c_str()).c_str());
    std::vector<Brew> brews;
    
    for (JsonVariant brew : doc.as<JsonArray>()) {
        Brew b;
        b.timestamp = brew["timestamp"] | 0UL;
        b.coffee = brew["coffee"] | "";
        b.in = brew["in"] | 0.0f;
        b.ratio = brew["ratio"] | 0.0f;
        b.aimOut = brew["aim_out"] | 0.0f;
        b.actualOut = brew["actual_out"] | 0.0f;
        b.grind = brew["grind"] | 0;
        b.brewTime = brew["brew_time"] | 0.0f;

        brews.push_back(b);
    }

    return brews;
}

void CoffeeDB::add(Brew brew) {
    JsonDocument doc;
    doc["timestamp"] = brew.timestamp;
    doc["coffee"] = brew.coffee;
    doc["in"] = brew.in;
    doc["ratio"] = brew.ratio;
    doc["actual_out"] = brew.actualOut;
    doc["grind"] = brew.grind;
    doc["brew_time"] = brew.brewTime;

    HTTPClient http;
    http.begin(String(secrets::coffeeDbEndpoint) + "/add");
    http.addHeader("Content-Type", "application/json");

    uint8_t buffer[512];
    size_t len = serializeJson(doc, buffer, sizeof(buffer));
    int code = http.POST(buffer, len);
    if (code != HTTP_CODE_OK) {
        LOG_INFO("HTTP POST failed with code: {}", code);
    }

    http.end();
}

JsonDocument CoffeeDB::httpGet(const std::string& url) {
    HTTPClient http;
    http.begin(url.c_str());

    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        LOG_INFO("HTTP GET failed with code: {}", code);
        http.end();
        return JsonDocument();
    }
    
    JsonDocument doc;
    deserializeJson(doc, http.getString());

    http.end();
    return doc;
}
