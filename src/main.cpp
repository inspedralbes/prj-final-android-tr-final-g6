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
char dateTime[20];
String apikey;

bool clockDisplayActive = false;
unsigned long clockDisplayStart = 0;
unsigned long lastDisplayUpdate = 0;

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

    configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "pool.ntp.org", "time.nist.gov");

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
        delay(2000);
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

    humidity::init();

    apikey = config::getApikey();
    Serial.println(apikey);
    String MAC = wifi::getMacAddress();
    Serial.printf("Adreça MAC del dispositiu: %s\n", MAC.c_str());
    String newApikey = connection::postNewSensor(MAC);
    Serial.print("Nova API Key: ");
    Serial.println(newApikey);
    Serial.printf("Nova API Key obtinguda: %s\n", newApikey.c_str());
    if (newApikey != apikey)
    {
        Serial.println("API Key canviada.");
        Serial.printf("API Key antiga: %s\n", apikey.c_str());
        Serial.printf("API Key nova: %s\n", newApikey.c_str());
        config::apikeyInsert(newApikey);
        apikey = newApikey;
    }
    Serial.println("API Key desada correctament.");
    Serial.println("Error: No s'ha pogut obtenir la API Key del servidor.");
}

void loop()
{
    if (apActive)
    {
        accesspoint::loopAccessPoint();
        return;
    }

    // Ajuste de brillo según el LDR
    if (ldr::checkChange())
    {
        uint8_t brightness = ldr::getBrightnessLevel();
        dma_display->setBrightness8(brightness);
        Serial.printf("Brillo ajustado a: %d\n", brightness);
    }

    static unsigned long lastSampleTime = 0;
    static unsigned long lastEmojiUpdateTime = 0;
    static unsigned long lastRequestTime = 0;

    static const int maxSamples = 10; // 500 ms * 10 = 5 segundos
    static float soundSamples[maxSamples];
    static float tempSamples[maxSamples];
    static int sampleCount = 0;

    static float lastSoundLevel = 0.0;
    static float lastTemperature = 0.0;

    // Toma de muestras cada 500ms
    if (millis() - lastSampleTime >= 500)
    {
        lastSampleTime = millis();

        float temp = temperature::readTemperatureSensor();
        float sound = sound::readSoundDataInDecibels();
        if (sampleCount == 0)
        {
            for (int i = 0; i < maxSamples; i++)
            {
            tempSamples[i] = temp;
            soundSamples[i] = sound;
            }
            sampleCount = maxSamples;
        }
        else if (sampleCount < maxSamples)
        {
            tempSamples[sampleCount] = temp;
            soundSamples[sampleCount] = sound;
            sampleCount++;
        }
        else
        {
            for (int i = 1; i < maxSamples; i++)
            {
            tempSamples[i - 1] = tempSamples[i];
            soundSamples[i - 1] = soundSamples[i];
            }
            tempSamples[maxSamples - 1] = temp;
            soundSamples[maxSamples - 1] = sound;
        }

        lastTemperature = temp;
        lastSoundLevel = sound;

        Serial.printf("Muestra %d -> Temp: %.2f °C, Sonido: %.2f dB\n", sampleCount, temp, sound);
    }

    // Solo cambia emoji si el reloj no está activo
    if (!clockDisplayActive && millis() - lastEmojiUpdateTime >= 1000)
    {
        lastEmojiUpdateTime = millis();

        float totalSound = 0.0;
        float totalTemp = 0.0;

        for (int i = 0; i < maxSamples; i++)
        {
            totalSound += soundSamples[i];
            totalTemp += tempSamples[i];
            Serial.printf("Muestra %d -> Temp: %.2f °C, Sonido: %.2f dB\n", i + 1, tempSamples[i], soundSamples[i]);
        }

        float avgSound = totalSound / maxSamples;
        float avgTemp = totalTemp / maxSamples;

        Serial.printf("Media Temp: %.2f °C | Media Sonido: %.2f dB\n", avgTemp, avgSound);
        emojis::changeEmoji(avgSound);
    }

    // Envío de datos cada segundo
    if (millis() - lastRequestTime >= 1000)
    {
        lastRequestTime = millis();

        float humitat = humidity::readHumidity();
        if (humitat >= 0)
        {
            Serial.printf("Humitat: %.2f%%\n", humitat);
        }
        else
        {
            Serial.println("Error llegint la humitat.");
        }

        Serial.printf("API Key carregada: %s\n", apikey.c_str());

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Error obtenint la data i hora actual.");
        }
        else
        {
            strftime(dateTime, sizeof(dateTime), "%Y-%m-%dT%H:%M:%S", &timeinfo);
            Serial.printf("Data i hora actual: %s\n", dateTime);
        }

        connection::postSensorData(apikey, lastSoundLevel, lastTemperature, humitat, dateTime, wifi::getMacAddress());
    }

    // Mostrar el reloj cada minuto durante 5 segundos
    if (!clockDisplayActive && millis() - lastDisplayUpdate >= 1000 * 60)
    {
        lastDisplayUpdate = millis();
        clockDisplayActive = true;
        clockDisplayStart = millis();

        Serial.println("Mostrando reloj...");

        float temp = temperature::readTemperatureSensor();
        float humitat = humidity::readHumidity();

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Error obtenint la data i hora actual.");
            return;
        }

        char timeStr[10];
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);

        dma_display->clearScreen();

        dma_display->fillRect(0, 0, 64, 16, dma_display->color565(0, 0, 50));
        dma_display->fillRect(0, 16, 64, 16, dma_display->color565(50, 0, 0));
        dma_display->fillRect(0, 32, 64, 16, dma_display->color565(0, 50, 0));
        dma_display->fillRect(0, 48, 64, 16, dma_display->color565(30, 30, 0));

        dma_display->setTextColor(dma_display->color565(255, 255, 255));
        dma_display->setTextSize(2);
        dma_display->setCursor(4, 2);
        dma_display->print(timeStr);

        dma_display->setTextSize(1);
        dma_display->setCursor(8, 18);
        dma_display->print("T:");
        dma_display->setCursor(24, 18);
        dma_display->printf("%.1fC", temp);

        dma_display->setCursor(8, 34);
        dma_display->print("H:");
        dma_display->setCursor(24, 34);
        dma_display->printf("%.1f%%", humitat);

        char dateStr[12];
        strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", &timeinfo);
        dma_display->setCursor(4, 50);
        dma_display->print(dateStr);
    }

    // Ocultar reloj tras 5 segundos
    if (clockDisplayActive && millis() - clockDisplayStart >= 5000)
    {
        Serial.println("Fin de visualización del reloj.");
        clockDisplayActive = false;
        dma_display->clearScreen(); // O dejar para que el emoji siguiente se dibuje
    }
}