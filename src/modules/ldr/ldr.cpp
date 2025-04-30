#include <Arduino.h>
#include "ldr.h"
#include "../../pins.h"
#include "../../modules/config/config.h"

namespace ldr {
    const int pinLDR = Pines::LDR;
    int indiceBrillo = 0;
    bool cambioDetectado = false;
    bool esperaCambio = false;

    void setup() {
        pinMode(pinLDR, INPUT);
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
}