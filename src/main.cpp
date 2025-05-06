#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FS.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "modules/wifi/wifi.h"
#include <WiFi.h>
#include "pins.h"
#include "config.h"
#include "modules/humidity/humidity.h"

#define ESP32_LED_BUILTIN 2

MatrixPanel_I2S_DMA *dma_display = nullptr;
const char *logos[] = {"/logo1.jpg", "/logo2.jpg", "/no.jpg", "/piola.jpg"};
bool connected = false;
int currentLogo = 0;
unsigned long lastChangeTime = 0;
bool showFractals = false;
int fractalIndex = 0;

const int LDR = Pines::LDR;

int indiceBrillo = 0;
bool cambioDetectado = false;

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
    dma_display->setRotation(displayRotation);
}

void mostrarImagen(const char *filename) {
    if (!SPIFFS.exists(filename)) return;
    File file = SPIFFS.open(filename, "r");
    if (!file) return;
    TJpgDec.drawFsJpg(0, 0, filename);
    file.close();
}

void setup() {
    Serial.begin(115200);
    pinMode(LDR, INPUT);
    pinMode(ESP32_LED_BUILTIN, OUTPUT);

    SPIFFS.begin(true);

    cargarConfig();

    displaySetup(brilloInicial, displayRotation);

    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tjpgDrawCallback);

    mostrarImagen(logos[currentLogo]);

    connected = wifi::connectToWiFi();

    if (connected) {
        mostrarImagen("/piola.jpg");
        delay(5000);
    } else {
        mostrarImagen("/no.jpg");
        delay(5000);
    }

    mostrarImagen(logos[currentLogo]);
    lastChangeTime = millis();

    humidity::init(); // Inicialitza el sensor de humitat
}

void loop() {
    int ldrValue = analogRead(LDR);
    static bool esperaCambio = false;

    if (ldrValue == 0 && !cambioDetectado && !esperaCambio) {
        cambioDetectado = true;
        esperaCambio = true;
        indiceBrillo = (indiceBrillo + 1) % numNiveles;
        dma_display->setBrightness8(nivelesBrillo[indiceBrillo]);
    }

    if (ldrValue > ldrThreshold) {
        cambioDetectado = false;
        esperaCambio = false;
    }

    if (!showFractals) {
        if (millis() - lastChangeTime >= logoDelay) {
            currentLogo = (currentLogo + 1) % 2;
            dma_display->clearScreen();
            mostrarImagen(logos[currentLogo]);
            lastChangeTime = millis();

            if (currentLogo == 1 && mostrarFractales) {
                showFractals = true;
                fractalIndex = 0;
            }
        }
    } else {
        dma_display->clearScreen();
        dma_display->setTextSize(1);
        dma_display->setTextColor(dma_display->color565(255, 255, 255));
        dma_display->setCursor(10, 28);
        String macAddress = WiFi.macAddress();
        dma_display->print(macAddress);
        delay(fractalDelay);
        showFractals = false;
    }

    float humitat = humidity::readHumidity();
    if (humitat >= 0) {
        Serial.print("Humitat: ");
        Serial.print(humitat);
        Serial.println("%");
    } else {
        Serial.println("Error llegint la humitat.");
    }
    delay(2000); // Llegeix cada 2 segons
}