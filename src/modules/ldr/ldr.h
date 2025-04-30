#ifndef LDR_H
#define LDR_H

namespace ldr {
    void setup();
    int read();
    bool checkChange();
    int getBrightnessLevel();
}

#endif