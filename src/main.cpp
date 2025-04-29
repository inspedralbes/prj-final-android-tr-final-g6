#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FS.h>
#include <SPIFFS.h>
#include <TJpg_Decoder.h>  // Para decodificar JPEG
#include "modules/wifi/wifi.h"
#include <WiFi.h>
#include "pins.h"

#define MIN_BRIGHT_DISPLAY_ON 4
#define MIN_BRIGHT_DISPLAY_OFF 0
#define ESP32_LED_BUILTIN 2

MatrixPanel_I2S_DMA *dma_display = nullptr;
const char *logos[] = {"/logo1.jpg", "/logo2.jpg", "/no.jpg", "/piola.jpg"};  // Archivos en SPIFFS
bool connected = false;
int currentLogo = 0;
unsigned long lastChangeTime = 0;
bool showFractals = false;  // Controla si mostrar fractales después de logo2.jpg
int fractalIndex = 0;       // Índice del fractal actual

// Callback para dibujar la imagen JPEG
bool tjpgDrawCallback(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    if (y >= 64) return false;
    dma_display->drawRGBBitmap(x, y, bitmap, w, h);
    return true;
}

void displaySetup(uint8_t displayBright, uint8_t displayRotation)
{
    HUB75_I2S_CFG mxconfig(64, 64, 1);

    // ✅ Configuración de pines desde pins.h
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
    dma_display->setRotation(displayRotation);
}

// Función para listar archivos en SPIFFS
void listarArchivosSPIFFS()
{
    Serial.println("\n📂 Archivos en SPIFFS:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        Serial.printf("📄 %s (%d bytes)\n", file.name(), file.size());
        file = root.openNextFile();
    }
}

// Cargar y mostrar imagen desde SPIFFS
void mostrarImagen(const char *filename)
{
    Serial.printf("🔍 Buscando archivo: %s\n", filename);

    if (!SPIFFS.exists(filename))
    {
        Serial.println("❌ Archivo no encontrado en SPIFFS.");
        return;
    }

    Serial.println("✅ Archivo encontrado, mostrando en la pantalla...");

    File file = SPIFFS.open(filename, "r");
    if (!file)
    {
        Serial.println("❌ Error al abrir el archivo.");
        return;
    }

    TJpgDec.drawFsJpg(0, 0, filename);
    file.close();
}

void setup()
{
    Serial.begin(115200);
    pinMode(ESP32_LED_BUILTIN, OUTPUT);

    Serial.println("\n🔄 Iniciando SPIFFS...");
    if (!SPIFFS.begin(true))
    {
        Serial.println("❌ Error al inicializar SPIFFS.");
        return;
    }
    Serial.println("✅ SPIFFS inicializado correctamente.");

    listarArchivosSPIFFS();  // Muestra los archivos disponibles en SPIFFS

    displaySetup(50, 0);

    // Configurar decodificador de JPEG
    TJpgDec.setJpgScale(1); // Escala 1:1 (tamaño original)
    TJpgDec.setCallback(tjpgDrawCallback);

    mostrarImagen(logos[currentLogo]);
    connected = wifi::connectToWiFi();

    if(connected)
    {
        Serial.println("✅ Conexión Wi-Fi exitosa.");
        mostrarImagen("/piola.jpg");
        delay(5000);
    }
    else
    {
        Serial.println("❌ Error al conectar a Wi-Fi.");
        mostrarImagen("/no.jpg");
        delay(5000);
    }
    // Mostrar el primer logo al iniciar
    mostrarImagen(logos[currentLogo]);
    lastChangeTime = millis();
}

void loop()
{
    if (!showFractals)
    {
        // Mostrar logos
        if (millis() - lastChangeTime >= 15000)
        {
            currentLogo = (currentLogo + 1) % 2;  // Alternar entre 0 y 1
            dma_display->clearScreen();
            mostrarImagen(logos[currentLogo]);
            lastChangeTime = millis();

            // Después de logo2.jpg, mostrar fractales
            if (currentLogo == 1)
            {
                showFractals = true;
                fractalIndex = 0;
            }
        }
    }
    else
    {
    dma_display->clearScreen();
    dma_display->setTextSize(1);
    dma_display->setTextColor(dma_display->color565(255, 255, 255)); // Color blanco
    dma_display->setCursor(10, 28);
    String macAddress = WiFi.macAddress();
    Serial.println("Dirección MAC: " + macAddress);
    dma_display->print(macAddress);

    // Reiniciar el ciclo para mostrar logos nuevamente después de un tiempo
    delay(5000); // Mostrar "Hola" durante 5 segundos
    showFractals = false;
    }
}