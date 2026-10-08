#include "v6o.hpp"

#include <WiFi.h>

#include "secrets.h"

namespace v6o {

constexpr uint32_t WIFI_TIMEOUT_MS = 15000;

bool connectWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start > WIFI_TIMEOUT_MS)
            return false;

        delay(250);
    }

    Serial.println(WiFi.localIP());

    return true;
}

}
