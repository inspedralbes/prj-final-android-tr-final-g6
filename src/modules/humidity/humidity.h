#pragma once
#include <Arduino.h>

namespace humidity {
    void init(); // Inicialitza el sensor
    float readHumidity(); // Llegeix el valor de la humitat
}