#include "config.h"
#include <FS.h>
#include <SPIFFS.h>

int brilloInicial = 50;
int ldrThreshold = 1000;
int displayRotation = 0;
int fractalDelay = 5000;
int logoDelay = 15000;
String wifiSSID = "";
String wifiPassword = "";
bool mostrarFractales = true;
int nivelesBrillo[5] = {0, 10, 50, 150, 255};
int numNiveles = 5;

void cargarConfig() {
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

    brilloInicial = doc["brilloInicial"] | brilloInicial;
    ldrThreshold = doc["ldrThreshold"] | ldrThreshold;
    displayRotation = doc["displayRotation"] | displayRotation;
    fractalDelay = doc["fractalDelay"] | fractalDelay;
    logoDelay = doc["logoDelay"] | logoDelay;
    wifiSSID = doc["wifi_ssid"] | wifiSSID;
    wifiPassword = doc["wifi_password"] | wifiPassword;
    mostrarFractales = doc["mostrarFractales"] | mostrarFractales;

    JsonArray niveles = doc["nivelesBrillo"];
    if (niveles && niveles.size() <= 5) {
        numNiveles = niveles.size();
        for (int i = 0; i < numNiveles; i++) {
            nivelesBrillo[i] = niveles[i];
        }
    }

    Serial.println("✅ Configuración cargada desde config.json.");
}