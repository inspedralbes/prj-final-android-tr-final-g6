#include <Arduino.h>
#include "ldr.h"
#include <ArduinoJson.h>
#include <TJpg_Decoder.h>
#include "../../pins.h"
#include "../../modules/config/config.h"

namespace ldr {
    const int pinLDR = Pines::LDR;
    int brightness = 0;
    bool chageDetected = false;
    bool waitChange = false;

    void setup() {
        pinMode(pinLDR, INPUT);
    }

    int read() {
        return analogRead(pinLDR);
    }

    bool checkChange() {
        int ldrValue = read();

        if (ldrValue == 0 && !chageDetected && !waitChange) {
            chageDetected = true;
            waitChange = true;
            brightness = (brightness + 1) % config::numNiveles;
            return true;
        }

        if (ldrValue > config::ldrThreshold) {
            chageDetected = false;
            waitChange = false;
        }

        return false;
    }

    int getBrightnessLevel() {
        return config::glowlevels[brightness];
    }

    void showImage(const char *filename)
{
    Serial.printf("Mostrant imatge: %s\n", filename);
    String fullPath = String("/images/") + filename;
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