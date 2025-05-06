#include "config.h"
#include <FS.h>
#include <SPIFFS.h>

namespace config {
    String date = "2023-10-01";
    int startglow = 50;
    int ldrThreshold = 1000;
    int displayRotation = 0;
    int fractalDelay = 5000;
    int logoDelay = 15000;
    String wifiSSID = "";
    String wifiPassword = "";
    bool mostrarFractales = true;
    int glowlevels[5] = {0};
    int numNiveles = 5;
    int db_good = 50;
    int db_normal = 70;
    int db_angry = 90;
    int db_very_angry = 110;
    String images[10] = {
        "good.jpg",
        "normal.jpg",
        "angry.jpg",
        "very_angry.jpg",
        "logo.jpg",
        "connected.jpg",
        "disconnected.jpg",
        "logo.jpg",
        "fractals.jpg",
        "fractals2.jpg"
    };


    void cargarConfig() {
        Serial.println("Cargando configuración desde config.json...");
        
        if (!SPIFFS.exists("/config.json")) {
            Serial.println("❌ No se encontró config.json. Usando valores por defecto.");
            return;
        }

        File configFile = SPIFFS.open("/config.json", "r");
        if (!configFile) {
            Serial.println("❌ No se pudo abrir config.json.");
            return;
        }

        size_t size = configFile.size();
        if (size > 2048) {
            Serial.println("❌ Config demasiado grande.");
            return;
        }

        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);

        StaticJsonDocument<2048> doc;
        auto error = deserializeJson(doc, buf.get());
        if (error) {
            Serial.println("❌ Error al parsear config.json.");
            return;
        }

        // Cargar valores desde config.json
        date = doc["date"] | date;
        startglow = doc["startglow"] | startglow;
        ldrThreshold = doc["ldrThreshold"] | ldrThreshold;
        fractalDelay = doc["fractalDelay"] | fractalDelay;
        logoDelay = doc["logoDelay"] | logoDelay;
        wifiSSID = doc["wifi_ssid"] | wifiSSID;
        wifiPassword = doc["wifi_password"] | wifiPassword;
        displayRotation = doc["displayRotation"] | displayRotation;
        mostrarFractales = doc["mostrarFractales"] | mostrarFractales;
        db_good = doc["db_good"] | db_good;
        db_normal = doc["db_normal"] | db_normal;
        db_angry = doc["db_angry"] | db_angry;
        db_very_angry = doc["db_very_angry"] | db_very_angry;

        JsonArray niveles = doc["glowlevels"];
        if (niveles && niveles.size() <= 5) {
            numNiveles = niveles.size();
            for (int i = 0; i < numNiveles; i++) {
                glowlevels[i] = niveles[i];
            }
        }

        JsonArray imgArray = doc["images"];
        if (imgArray && imgArray.size() <= 10) {
            for (int i = 0; i < imgArray.size(); i++) {
            images[i] = imgArray[i].as<String>();
            }
        }

        Serial.print("Niveles de brillo: ");
        for (int i = 0; i < numNiveles; i++) {
            Serial.print(glowlevels[i]);
            if (i < numNiveles - 1) Serial.print(", ");
        }

        Serial.println("✅ Configuración cargada desde config.json.");
    }
}