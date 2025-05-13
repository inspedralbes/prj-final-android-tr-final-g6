#include <Arduino.h>
#include "ldr.h"
#include <ArduinoJson.h>
#include <TJpg_Decoder.h>
#include <AnimatedGIF.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>

#include "../../pins.h"
#include "../../modules/config/config.h"

namespace ldr {
    const int pinLDR = Pines::LDR;
    int indiceBrillo = 0;
    bool cambioDetectado = false;
    bool esperaCambio = false;

    TFT_eSPI tft = TFT_eSPI();
    AnimatedGIF gif;

    void setup() {
        pinMode(pinLDR, INPUT);
        tft.begin();
        tft.setRotation(1);
        gif.begin(LITTLE_ENDIAN_PIXELS);
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
        return config::glowLevels[indiceBrillo];
    }

    void *GIFDraw(GIFDRAW *pDraw) {
        if (pDraw->iY >= tft.height()) return nullptr;
        tft.pushImage(pDraw->iX, pDraw->iY, pDraw->iWidth, 1, (uint16_t *)pDraw->pPixels);
        return nullptr;
    }

    void showImage(const char *filename)
    {
        Serial.printf("Mostrant imatge: %s\n", filename);
        String fullPath = String("/images/") + filename;
        Serial.printf("Ruta completa: %s\n", fullPath.c_str());

        if (!SPIFFS.exists(fullPath)) {
            Serial.println("El archivo no existe.");
            return;
        }

        File file = SPIFFS.open(fullPath, "r");
        if (!file) {
            Serial.println("No se pudo abrir el archivo.");
            return;
        }

        String lowerName = String(filename);
        lowerName.toLowerCase();

        if (lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg")) {
            TJpgDec.drawFsJpg(0, 0, fullPath.c_str());
        } else if (lowerName.endsWith(".gif")) {
            if (gif.open(&file, GIFDraw)) {
                while (gif.playFrame(true, NULL)) {
                    delay(gif.getFrameDelayMs());
                }
                gif.close();
            } else {
                Serial.println("No se pudo decodificar el GIF.");
            }
        } else {
            Serial.println("Formato no soportado.");
        }

        file.close();
    }
}