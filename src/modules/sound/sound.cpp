#include "pins.h"
#include <Arduino.h>
namespace sound {
    void initSoundSensor() {
        pinMode(Pines::MICROFONO, INPUT);
    }

    // Leer el nivel de sonido y devolverlo en decibelios
    float readSoundDataInDecibels() {
        // Leer el valor analógico del micrófono
        int soundValue = analogRead(Pines::MICROFONO);

        // Convertir el valor a decibelios (ajustar la fórmula según sea necesario)
        float decibels = 20 * log10(soundValue / 4095.0); // 4095 es el valor máximo para un ADC de 12 bits

        return decibels;
    }

}