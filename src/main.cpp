#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FS.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>
#include "modules/wifi/wifi.h"
#include "pins.h"
#include "modules/config/config.h"
#include "modules/temperature/temperature.h"
#include "modules/ldr/ldr.h"
#include "modules/sound/sound.h"
#include "modules/humidity/humidity.h"
#include "modules/download/download.h"
#include "modules/connection/connection.h"
#include "modules/accesspoint/accesspoint.h"
#include "modules/emojis/emojis.h"

#define ESP32_LED_BUILTIN 2

MatrixPanel_I2S_DMA *dma_display = nullptr;
int currentLogo = 0;
unsigned long lastChangeTime = 0;

bool connected = false;
bool apActive = false;
String apikey;

bool tjpgDrawCallback(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    if (y >= 64)
        return false;
    dma_display->drawRGBBitmap(x, y, bitmap, w, h);
    return true;
}

void displaySetup(uint8_t displayBright, uint8_t displayRotation)
{
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
    mxconfig.gpio.oe = Pines::MATRIX_OE;

    mxconfig.clkphase = false;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(displayBright);
    dma_display->clearScreen();
    dma_display->setRotation(config::displayRotation);
}

void setup()
{
    Serial.begin(115200);
    pinMode(ESP32_LED_BUILTIN, OUTPUT);

    SPIFFS.begin(true);
    config::cargarConfig();

    displaySetup(config::startglow, config::displayRotation);

    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback(tjpgDrawCallback);

    Serial.println("Configuració de la pantalla completada.");

    ldr::showImage("logo.jpg");

    ldr::setup();

    config::cargarWifi();

    connected = wifi::connectToWiFi();

    if (connected)
    {
        ldr::showImage("connected.jpg");
        download::downloadConfig();
        config::cargarConfig();
        download::downloadImages();
        delay(5000);
    }
    else
    {
        Serial.println("No s'ha pogut connectar a la xarxa WiFi. Creant Access Point.");
        ldr::showImage("disconnected.jpg");
        delay(2000);
        ldr::showImage("accesspoint.jpg");
        accesspoint::setupAccessPoint();
        apActive = true;
    }

    lastChangeTime = millis();

    humidity::init(); // Inicialitza el sensor de humitat
}

void loop()
{
    if (apActive)
    {
        accesspoint::loopAccessPoint();
        return;
    }

    if (ldr::checkChange())
    {
        uint8_t brightness = ldr::getBrightnessLevel();
        dma_display->setBrightness8(brightness);
        Serial.printf("Brillo ajustado a: %d\n", brightness);
    }

    static unsigned long lastActionTime = 0;
    static unsigned long lastRequestTime = 0;

    if (millis() - lastActionTime >= 1000)
    {
        lastActionTime = millis();

        // Lectura de temperatura
        float temperature = temperature::readTemperatureSensor();
        Serial.printf("Temperatura: %.2f °C\n", temperature);

        // Lectura de nivel de sonido
        float soundLevel = sound::readSoundDataInDecibels();
        Serial.printf("Nivell de so: %d\n", soundLevel);

        // Actualiza emoji en función del nivel de sonido
        emojis::changeEmoji(soundLevel);

        // Lectura de humedad
        float humitat = humidity::readHumidity();
        if (humitat >= 0)
        {
            Serial.print("Humitat: ");
            Serial.print(humitat);
            Serial.println("%");
        }
        else
        {
            Serial.println("Error llegint la humitat.");
        }

        apikey = config::getApikey();
        Serial.println(apikey); // ← se añadió punto y coma

        if (apikey == "EXAMPLE")
        {
                lastRequestTime = millis();
                Serial.println("No s'ha trobat cap API Key vàlida a la configuració. Descarregant nova configuració...");
                String MAC = wifi::getMacAddress();
                Serial.printf("Adreça MAC del dispositiu: %s\n", MAC.c_str());
                String newApikey = connection::postNewSensor(MAC);
                Serial.print("Nova API Key: ");
                Serial.println(newApikey);

                if (newApikey != "" || newApikey != "EXAMPLE")
                {
                    Serial.printf("Nova API Key obtinguda: %s\n", newApikey.c_str());
                    config::apikeyInsert(newApikey);
                    apikey = newApikey;
                    Serial.println("API Key desada correctament.");
                }
                else
                {
                    Serial.println("Error: No s'ha pogut obtenir la API Key del servidor.");
                }
        }
        else
        {
            Serial.printf("API Key carregada: %s\n", apikey.c_str());
            connection::postSensorData(apikey, soundLevel, temperature, humitat, wifi::getMacAddress());
        }
    }
}