#ifndef GIF_H
#define GIF_H

#include <MatrixPanel_I2S_DMA.h>
#include <string>

class GifHandler {
private:
    MatrixPanel_I2S_DMA *display;
    std::string filePath;

public:
    GifHandler(MatrixPanel_I2S_DMA *display, const std::string &filePath);
    void displayGif();
};

#endif // GIF_H