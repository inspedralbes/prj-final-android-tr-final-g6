#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FS.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "modules/wifi/wifi.h"
#include <WiFi.h>
#include "pins.h"
#include "modules/config/config.h"
#include "modules/temperature/temperature.h"
#include "modules/ldr/ldr.h"

#define ESP32_LED_BUILTIN 2

MatrixPanel_I2S_DMA *dma_display = nullptr;
bool connected = false;
int currentLogo = 0;
unsigned long lastChangeTime = 0;

bool tjpgDrawCallback(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    if (y >= 64) return false;
    dma_display->drawRGBBitmap(x, y, bitmap, w, h);
    return true;
}

void displaySetup(uint8_t displayBright, uint8_t displayRotation) {
    HUB75_I2S_CFG mxconfig(64, 64, 1);

    mxconfig.gpio.r1 = Pines::MATRIX_R1;
    mxconfig.gpio.g1 = Pines::MATRIX_G1;
    mxconfig.gpio.b1 = Pines::MATRIX_B1;
    mxconfig.gpio.r2 = Pines::MATRIX_R2;
    mxconfig.gpio.g2 = Pines::MATRIX_G2;
    mxconfig.gpio.b2 = Pines::MATRIX_B2;
    mxconfig.gpio.a = Pines::MATRIX_A;
    mxconfig.gpio.b = Pines::MATRIX_B;
    mxconfig.gpio.c = Pines::MATRIX_C;
    mxconfig.gpio.d = Pines::MATRIX_D;
    mxconfig.gpio.e = Pines::MATRIX_E;
    mxconfig.gpio.clk = Pines::MATRIX_CLK;
    mxconfig.gpio.lat = Pines::MATRIX_LAT;
    mxconfig.gpio.oe  = Pines::MATRIX_OE;

    mxconfig.clkphase = false;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(displayBright);
    dma_display->clearScreen();
    dma_display->setRotation(config::displayRotation);
}

void showImage(const char *filename) {
    Serial.printf("Mostrant imatge: %s\n", filename);
    String fullPath = String("/config::images/") + filename;
    Serial.printf("Mostrant imatge: %s\n", fullPath);
    if (!SPIFFS.exists(fullPath)) return;
    File file = SPIFFS.open(fullPath, "r");
    if (!file) return;
    TJpgDec.drawFsJpg(0, 0, filename);
    file.close();
}

void setup() {
    Serial.begin(115200);
    pinMode(ESP32_LED_BUILTIN, OUTPUT);

    SPIFFS.begin(true);
    config::cargarConfig();

    Serial.println("Llista d'imatges disponibles:");
    for (int i = 0; i < sizeof(config::images) / sizeof(config::images[0]); i++) {
        Serial.println(config::images[i]);
    }

    displaySetup(config::startglow, config::displayRotation);

    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tjpgDrawCallback);

    showImage(config::images[currentLogo]);

    ldr::setup();

    connected = wifi::connectToWiFi();

    if (connected) {
        showImage(config::images[1]);
        delay(5000);
    } else {
        showImage(config::images[2]);
        delay(5000);
    }

    showImage(config::images[currentLogo]);
    lastChangeTime = millis();
}

void loop() {
    if (ldr::checkChange()) {
        dma_display->setBrightness8(ldr::getBrightnessLevel());
    }

    float temperature = temperature::readTemperatureSensor();
    // Serial.print("Temperatura: ");
    // Serial.println(temperature, 2);

    if (millis() - lastChangeTime >= config::logoDelay) {
        currentLogo = (currentLogo + 1) % 2;
        dma_display->clearScreen();
        showImage(config::images[currentLogo]);
        lastChangeTime = millis();
    }
}
