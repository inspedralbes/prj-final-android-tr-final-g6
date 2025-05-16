#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "../config/config.h"

namespace wifi {

    bool connectToWiFi() {
        Serial.print("Connectant a Wi-Fi: ");
        Serial.println(config::wifiSSID);

        if (config::wifiPassword == "") {
            Serial.println("Sense contrasenya.");
            WiFi.begin(config::wifiSSID.c_str());
        } else {
            Serial.println("Amb contrasenya.");
            WiFi.begin(config::wifiSSID.c_str(), config::wifiPassword.c_str());
        }

        unsigned long startAttemptTime = millis();
        const unsigned long timeout = 10000; // 10 segundos

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWi-Fi connectat.");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            return true;
        } else {
            Serial.println("\nError a la conexión Wi-Fi.");
            Serial.print("Estat del Wi-Fi: ");
            Serial.println(WiFi.status());
            switch (WiFi.status()) {
                case WL_NO_SSID_AVAIL:
                    Serial.println("SSID no disponible.");
                    break;
                case WL_CONNECT_FAILED:
                    Serial.println("Error a la connexió. Verifica la contrasenya.");
                    break;
                case WL_DISCONNECTED:
                    Serial.println("Wi-Fi desconnectat.");
                    break;
                default:
                    Serial.println("Error desconegut.");
                    break;
            }
            return false;
        }
    }
    String getMacAddress() {
        uint8_t mac[6];
        WiFi.macAddress(mac);
        String macStr = String(mac[0], HEX) + ":" +
                        String(mac[1], HEX) + ":" +
                        String(mac[2], HEX) + ":" +
                        String(mac[3], HEX) + ":" +
                        String(mac[4], HEX) + ":" +
                        String(mac[5], HEX);
        return macStr;
    }
    String getIp() {
        IPAddress ip = WiFi.localIP();
        String ipStr = String(ip[0]) + "." +
                       String(ip[1]) + "." +
                       String(ip[2]) + "." +
                       String(ip[3]);
        return ipStr;
    }
}