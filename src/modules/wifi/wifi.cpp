#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "wifi.h"

namespace wifi {
    bool loadWiFiConfig(WiFiConfig& config) {
        if (!SPIFFS.begin(true)) {
            Serial.println("❌ Error al montar el sistema de archivos SPIFFS.");
            return false;
        }

        File configFile = SPIFFS.open("/config.json", "r");
        if (!configFile) {
            Serial.println("❌ No se pudo abrir el archivo Wi-Fi.");
            return false;
        }

        size_t size = configFile.size();
        if (size > 1024) { // Limita el tamaño del archivo JSON
            Serial.println("❌ El archivo de configuración es demasiado grande.");
            return false;
        }

        // Leer el archivo JSON
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);

        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, buf.get());
        if (error) {
            Serial.println("❌ Error al analizar el archivo JSON.");
            return false;
        }

        // Extraer SSID y contraseña
        if (!doc.containsKey("wifi_ssid") || !doc.containsKey("wifi_password")) {
            Serial.println("❌ El archivo JSON no contiene las claves necesarias.");
            return false;
        }

        config.ssid = doc["wifi_ssid"].as<String>();
        config.password = doc["wifi_password"].as<String>();

        Serial.println("✅ Configuración Wi-Fi cargada:");
        Serial.println("SSID: " + config.ssid);
        Serial.println("Contraseña: " + config.password);
        return true;
    }

    bool connectToWiFi() {
        WiFiConfig config;
        if (!loadWiFiConfig(config)) {
            Serial.println("❌ Error al cargar configuración Wi-Fi.");
            return false;
        }

        Serial.print("🔄 Conectando a ");
        Serial.println(config.ssid);

        WiFi.begin(config.ssid.c_str(), config.password.c_str());

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