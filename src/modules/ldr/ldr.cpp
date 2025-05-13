#include <Arduino.h>
#include "ldr.h"
#include <ArduinoJson.h>
#include <AnimatedGIF.h>
#include <SPIFFS.h>
#include "../../pins.h"
#include "../../modules/config/config.h"

namespace ldr {
    const int pinLDR = Pines::LDR;
    int indiceBrillo = 0;
    bool cambioDetectado = false;
    bool esperaCambio = false;

    // Objeto para manejar GIFs animados
    AnimatedGIF gif;
    File gifFile;
    unsigned long lastFrameTime = 0;

    // Declaraciones adelantadas de las funciones privadas
    static void* GIFOpenFile(const char* fname, int32_t* pSize);
    static void GIFCloseFile(void* pHandle);
    static int32_t GIFReadFile(GIFFILE* pFile, uint8_t* pBuf, int32_t iLen);
    static int32_t GIFSeekFile(GIFFILE* pFile, int32_t iPosition);
    static void GIFDraw(GIFDRAW* pDraw);
    void showGIF(const char* filename);

    void setup() {
        pinMode(pinLDR, INPUT);
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
        return config::glowlevels[indiceBrillo];
    }

    // Función para dibujar cada frame del GIF
    static void GIFDraw(GIFDRAW* pDraw) {
        uint8_t* s;
        uint16_t* usPalette;
        int x, y, iWidth;

        iWidth = pDraw->iWidth;
        if (iWidth > 64) iWidth = 64;
        
        usPalette = pDraw->pPalette;
        y = pDraw->iY + pDraw->y; // Posición Y absoluta
        
        if (y >= 64 || y < 0) return; // Fuera de los límites
            
        s = pDraw->pPixels;
        
        // Si hay transparencia, aplicarla
        if (pDraw->ucHasTransparency) {
            for (x = 0; x < iWidth; x++) {
                if (pDraw->ucTransparent != s[x] && (pDraw->iX + x) < 64) {
                    // Dibujar el píxel en la pantalla LED
                    // Implementación específica para tu hardware:
                    // Ejemplo para FastLED:
                    // leds[XY(pDraw->iX + x, y)] = CRGB(
                    //     (usPalette[s[x]] >> 8) & 0xF8,  // R
                    //     (usPalette[s[x]] >> 3) & 0xFC,  // G
                    //     (usPalette[s[x]] << 3) & 0xF8); // B
                }
            }
        } else {
            for (x = 0; x < iWidth; x++) {
                if ((pDraw->iX + x) < 64) {
                    // Dibujar el píxel sin transparencia
                    // Implementación similar al caso anterior
                }
            }
        }
    }

    void showImage(const char* filename) {
        // Verificar extensión para determinar si es JPG o GIF
        String strFilename(filename);
        if (strFilename.endsWith(".gif") || strFilename.endsWith(".GIF")) {
            showGIF(filename);
        } else {
            // Aquí iría tu código original para mostrar JPG
            // showJPG(filename);
        }
    }

    void showGIF(const char* filename) {
        Serial.printf("Mostrando GIF: %s\n", filename);
        String fullPath = String("/images/") + filename;
        
        if (!SPIFFS.exists(fullPath)) {
            Serial.println("El archivo no existe.");
            return;
        }
        
        if (gif.open(fullPath.c_str(), GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
            Serial.printf("Tamaño del GIF: %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
            
            while (gif.playFrame(true, NULL)) {
                // Ajustar brillo según el LDR
                if (checkChange()) {
                    // Actualizar brillo si hubo cambio
                    // FastLED.setBrightness(getBrightnessLevel());
                }
                
                delay(2000); // Usar el delay del frame del GIF
            }
            
            gif.close();
        } else {
            Serial.println("Error al abrir el GIF");
        }
    }

    // Implementación de las funciones de callback
    static void* GIFOpenFile(const char* fname, int32_t* pSize) {
        File* file = new File(SPIFFS.open(fname));
        if (*file) {
            *pSize = file->size();
            return (void*)file;
        }
        delete file;
        return NULL;
    }

    static void GIFCloseFile(void* pHandle) {
        File* file = static_cast<File*>(pHandle);
        if (file != NULL) {
            file->close();
            delete file;
        }
    }

    static int32_t GIFReadFile(GIFFILE* pFile, uint8_t* pBuf, int32_t iLen) {
        File* file = static_cast<File*>(pFile->fHandle);
        if (iLen > (int32_t)(pFile->iSize - pFile->iPos))
            iLen = pFile->iSize - pFile->iPos;
        if (iLen <= 0)
            return 0;
        iLen = (int32_t)file->read(pBuf, iLen);
        pFile->iPos = file->position();
        return iLen;
    }

    static int32_t GIFSeekFile(GIFFILE* pFile, int32_t iPosition) {
        File* file = static_cast<File*>(pFile->fHandle);
        file->seek(iPosition);
        pFile->iPos = file->position();
        return pFile->iPos;
    }
}