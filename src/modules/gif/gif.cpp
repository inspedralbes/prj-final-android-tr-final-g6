#include "gif.h"
#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include <iostream>

// Constructor
GifHandler::GifHandler(MatrixPanel_I2S_DMA *display, const std::string &filePath)
    : display(display), filePath(filePath) {}

// Método para cargar y mostrar el GIF
void GifHandler::displayGif() {
    File gifFile = SPIFFS.open(filePath.c_str(), "r");
    if (!gifFile) {
        Serial.printf("Error: No se pudo abrir el archivo GIF: %s\n", filePath.c_str());
        return;
    }

    AnimatedGIF gif;
    gif.begin(LITTLE_ENDIAN_PIXELS);
    gif.open(gifFile, [](void *p, uint8_t *buf, int len) -> int {
        return ((File *)p)->read(buf, len);
    });

    while (gif.playFrame()) {
        uint16_t *frameBuffer = (uint16_t *)gif.getPixels();
        display->drawRGBBitmap(0, 0, frameBuffer, gif.getWidth(), gif.getHeight());
        delay(gif.getDelay());
    }

    gif.close();
    gifFile.close();
}