#include "config.h"
#include <FS.h>
#include <SPIFFS.h>

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
const char* images[10] = {"/logo1.jpg"};

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
    startglow = doc["startglow"] | startglow;
    ldrThreshold = doc["ldrThreshold"] | ldrThreshold;
    fractalDelay = doc["fractalDelay"] | fractalDelay;
    logoDelay = doc["logoDelay"] | logoDelay;
    wifiSSID = doc["wifi_ssid"] | wifiSSID;
    wifiPassword = doc["wifi_password"] | wifiPassword;
    displayRotation = doc["displayRotation"] | displayRotation;
    mostrarFractales = doc["mostrarFractales"] | mostrarFractales;

    JsonArray niveles = doc["glowlevels"];
    if (niveles && niveles.size() <= 5) {
        numNiveles = niveles.size();
        for (int i = 0; i < numNiveles; i++) {
            glowlevels[i] = niveles[i];
        }
    }

    Serial.print("Niveles de brillo: ");
    for (int i = 0; i < numNiveles; i++) {
        Serial.print(glowlevels[i]);
        if (i < numNiveles - 1) Serial.print(", ");
    }

    JsonArray imagesArray = doc["images"];
    if (imagesArray && imagesArray.size() <= sizeof(images) / sizeof(images[0])) {
        for (size_t i = 0; i < imagesArray.size(); i++) {
            images[i] = imagesArray[i].as<const char*>();
        }
    }

    for (size_t i = 0; i < sizeof(images) / sizeof(images[0]); i++) {
        if (images[i] != nullptr) {
            Serial.println(images[i]);
        }
    }

    Serial.println("✅ Configuración cargada desde config.json.");
}