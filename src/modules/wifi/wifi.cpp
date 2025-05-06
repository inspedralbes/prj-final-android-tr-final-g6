#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "../config/config.h"

namespace wifi {

    bool connectToWiFi() {
        Serial.print("🔄 Conectando a ");
        Serial.println(config::wifiSSID);

        WiFi.begin(config::wifiSSID.c_str(), config::wifiPassword.c_str());

        unsigned long startAttemptTime = millis();
        const unsigned long timeout = 10000; // 10 segundos

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ Wi-Fi conectado.");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            return true;
        } else {
            Serial.println("\n❌ Falló la conexión Wi-Fi.");
            Serial.print("Estado de Wi-Fi: ");
            Serial.println(WiFi.status());
            switch (WiFi.status()) {
                case WL_NO_SSID_AVAIL:
                    Serial.println("⚠️ SSID no disponible.");
                    break;
                case WL_CONNECT_FAILED:
                    Serial.println("⚠️ Falló la conexión. Verifica la contraseña.");
                    break;
                case WL_DISCONNECTED:
                    Serial.println("⚠️ Wi-Fi desconectado.");
                    break;
                default:
                    Serial.println("⚠️ Error desconocido.");
                    break;
            }
            return false;
        }
    }
}