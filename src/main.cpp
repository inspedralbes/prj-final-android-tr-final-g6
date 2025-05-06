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
#include "modules/sound/sound.h"
#include "modules/humidity/humidity.h"

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

    Serial.println();

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
    TJpgDec.drawFsJpg(0, 0, fullPath.c_str());
    file.close();
}

void setup() {
    Serial.begin(115200);
    pinMode(ESP32_LED_BUILTIN, OUTPUT);

    SPIFFS.begin(true);
    config::cargarConfig();

    displaySetup(config::startglow, config::displayRotation);

    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tjpgDrawCallback);

    Serial.println("Configuració de la pantalla completada.");

    showImage("logo.jpg");

    ldr::setup();

    connected = wifi::connectToWiFi();

    if (connected) {
        showImage("connected.jpg");
        delay(5000);
    } else {
        showImage("disconnected.jpg");
        delay(5000);
    }

    showImage("logo.jpg");
    lastChangeTime = millis();

    humidity::init(); // Inicialitza el sensor de humitat
}

void loop() {
    if (ldr::checkChange()) {
        // Obtén el nivel de brillo del sensor y actualiza la pantalla
        uint8_t brightness = ldr::getBrightnessLevel();
        dma_display->setBrightness8(brightness);
        Serial.printf("Brillo ajustado a: %d\n", brightness);
    }

    static unsigned long lastActionTime = 0;
    if (millis() - lastActionTime >= 1000) {
        lastActionTime = millis();

        // Lee la temperatura (puedes usarla si es necesario)
        float temperature = temperature::readTemperatureSensor();
        Serial.printf("Temperatura: %.2f °C\n", temperature);

        // Lee el nivel de sonido y muestra la imagen correspondiente
        float soundLevel = sound::readSoundDataInDecibels();
        Serial.printf("Nivell de so: %d\n", soundLevel);

        if (soundLevel < 30) {
            showImage("good.jpg");
        } else if (soundLevel < 70) {
            showImage("normal.jpg");
        } else {
            showImage("angry.jpg");
        }
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
