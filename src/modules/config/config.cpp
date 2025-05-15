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
    float vref_sound = 0.7;
    String images[10] = {
        "good.jpg",
        "normal.jpg",
        "angry.jpg",
        "very_angry.jpg",
        "logo.jpg",
        "connected.jpg"
    };
    String url_newsensor = "";
    String url_sensor = "";
    int maxSamples = 10;


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
        displayRotation = doc["displayRotation"] | displayRotation;
        mostrarFractales = doc["mostrarFractales"] | mostrarFractales;
        db_good = doc["db_good"] | db_good;
        db_normal = doc["db_normal"] | db_normal;
        db_angry = doc["db_angry"] | db_angry;
        url_newsensor = doc["url_newsensor"] | url_newsensor;
        url_sensor = doc["url_sensor"] | url_sensor;
        vref_sound = doc["vref_sound"] | vref_sound;
        maxSamples = doc["maxSamples"] | maxSamples;

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
        Serial.println("\n--- Configuración actual ---");
        Serial.println("Fecha: " + date);
        Serial.println("Start Glow: " + String(startglow));
        Serial.println("LDR Threshold: " + String(ldrThreshold));
        Serial.println("Fractal Delay: " + String(fractalDelay));
        Serial.println("Logo Delay: " + String(logoDelay));
        Serial.println("Display Rotation: " + String(displayRotation));
        Serial.println("Mostrar Fractales: " + String(mostrarFractales));
        Serial.println("DB Good: " + String(db_good));
        Serial.println("DB Normal: " + String(db_normal));
        Serial.println("DB Angry: " + String(db_angry));
        Serial.println("vref_sound: " + String(vref_sound));
        Serial.println("URL New Sensor: " + url_newsensor);
        Serial.println("URL Sensor: " + url_sensor);
        Serial.println("WiFi SSID: " + wifiSSID);
        Serial.println("WiFi Password: " + wifiPassword);
        Serial.println("Max Samples: " + String(maxSamples));

        Serial.print("Niveles de brillo: ");
        for (int i = 0; i < numNiveles; i++) {
            Serial.print(glowlevels[i]);
            if (i < numNiveles - 1) Serial.print(", ");
        }
        Serial.println();

        Serial.println("Imágenes:");
        for (int i = 0; i < 10; i++) {
            if (images[i] != "") {
            Serial.println(" - " + images[i]);
            }
        }
        Serial.println("----------------------------");
    }

    void cargarWifi(){
        Serial.println("Cargando configuración desde wifi.json...");
        
        if (!SPIFFS.exists("/wifi.json")) {
            Serial.println("❌ No se encontró config.json. Usando valores por defecto.");
            return;
        }

        File configFile = SPIFFS.open("/wifi.json", "r");
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
            Serial.println("❌ Error al parsear wifi.json.");
            return;
        }

        wifiSSID = doc["wifi_ssid"] | wifiSSID;
        wifiPassword = doc["wifi_password"] | wifiPassword;
    }

    void apikeyInsert(String apikey) {
        StaticJsonDocument<256> doc;
        doc["apikey"] = apikey;

        File file = SPIFFS.open("/apikey.json", "w");
        if (!file) {
            Serial.println("❌ No se pudo crear apikey.json.");
            return;
        }

        if (serializeJson(doc, file) == 0) {
            Serial.println("❌ Error al escribir en apikey.json.");
        } else {
            Serial.println("✅ API Key guardada en apikey.json.");
        }

        file.close();
    }

    String getApikey() {
        if (!SPIFFS.exists("/apikey.json")) {
            Serial.println("❌ No se encontró apikey.json.");
            return "";
        }

        File file = SPIFFS.open("/apikey.json", "r");
        if (!file) {
            Serial.println("❌ No se pudo abrir apikey.json.");
            return "";
        }

        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, file);

        if (error) {
            Serial.println("❌ Error al leer apikey.json.");
            file.close();
            return "";
        }

        String apikey = doc["apikey"].as<String>();
        if (apikey.isEmpty()) {
            Serial.println("❌ API Key no encontrada en apikey.json.");
        } else {
            Serial.println("✅ API Key leída correctamente.");
            Serial.println("API Key: " + apikey);
        }

        file.close();

        return apikey;
    }

}