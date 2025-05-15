#include <Arduino.h>
#include "ldr.h"
#include <ArduinoJson.h>
#include <TJpg_Decoder.h>
#include "../../pins.h"
#include "../../modules/config/config.h"

namespace ldr {
    const int pinLDR = Pines::LDR;
    int indiceBrillo = 0;
    bool cambioDetectado = false;
    bool esperaCambio = false;

    void setup() {
        pinMode(pinLDR, INPUT);
    }

    int read() {
        return analogRead(pinLDR);
    }

    bool checkChange() {
        int ldrValue = read();

        if (ldrValue == 0 && !cambioDetectado && !esperaCambio) {
            cambioDetectado = true;
            esperaCambio = true;
            indiceBrillo = (indiceBrillo + 1) % config::numNiveles;
            return true;
        }

        if (ldrValue > config::ldrThreshold) {
            cambioDetectado = false;
            esperaCambio = false;
        }

        return false;
    }

    int getBrightnessLevel() {
        return config::glowlevels[indiceBrillo];
    }

    void showImage(const char *filename)
{
    Serial.printf("Mostrant imatge: %s\n", filename);
    String fullPath = String("/images/") + filename;
    Serial.printf("Ruta completa: %s\n", fullPath.c_str());
    if (!SPIFFS.exists(fullPath))
    {
        Serial.println("El arxiu no existeix.");
        return;
    }
    File file = SPIFFS.open(fullPath, "r");
    if (!file)
    {
        Serial.println("No s'ha pogut obrir el fitxer.");
        return;
    }
    TJpgDec.drawFsJpg(0, 0, fullPath.c_str());
    file.close();
}
}