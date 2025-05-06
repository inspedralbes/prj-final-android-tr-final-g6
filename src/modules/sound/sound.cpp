#include "pins.h"
#include <Arduino.h>
namespace sound
{
    void initSoundSensor()
    {
        pinMode(Pines::MICROFONO, INPUT);
    }

    const int numMuestras = 100;
    const float Vref = 0.07;
    const int umbral = 10;

    float readSoundDataInDecibels()
    {
        double suma = 0;
        int lectura;

        for (int i = 0; i < numMuestras; i++)
        {
            lectura = analogRead(Pines::MICROFONO);

            if (lectura < umbral)
            {
                continue;
            }

            suma += (lectura * lectura);
        }

        float Vrms = sqrt(suma / (float)numMuestras);

        Serial.println("Vrms: " + String(Vrms));

        if (Vrms < 1.0)
            Vrms = 1.0;

        float dB = 20.0 * log10(Vrms / Vref);

        Serial.println("dB: " + String(dB));

        return dB;
    }
}