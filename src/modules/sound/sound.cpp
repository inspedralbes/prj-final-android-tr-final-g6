#include "pins.h"
#include <Arduino.h>
namespace sound
{
    void initSoundSensor()
    {
        pinMode(Pines::MICROFONO, INPUT);
    }

    // Leer el nivel de sonido y devolverlo en decibelios
    const int numMuestras = 100;
    const float Vref = 10.0; // Ajusta esto según tu entorno

    float readSoundDataInDecibels()
    {
        double suma = 0;
        int lectura;

        // Tomar múltiples muestras para obtener un promedio
        for (int i = 0; i < numMuestras; i++)
        {
            // Leer el valor analógico
            lectura = analogRead(Pines::MICROFONO);

            // Asegúrate de que la lectura esté en el rango esperado
            if (lectura < 0 || lectura > 1023)
            {
                Serial.println("Lectura fuera de rango: " + String(lectura));
                continue; // Salta esta iteración si la lectura es inválida
            }

            // Centrar la lectura en 0 (ajusta si es necesario)
            lectura -= 512;            // Esto asume que el micrófono está centrado en 0V
            suma += lectura * lectura; // Sumar el cuadrado de la lectura
        }

        // Calcular Vrms
        float Vrms = sqrt(suma / (float)numMuestras);

        // Imprimir Vrms para depuración
        Serial.println("Vrms: " + String(Vrms));

        // Evitar log10 de 0 o valores negativos
        if (Vrms < 1.0)
            Vrms = 1.0; // Asegúrate de que Vrms no sea menor que 1.0

        // Calcular el nivel en decibelios
        float dB = 20.0 * log10(Vrms / Vref);

        // Imprimir el resultado en dB para depuración
        Serial.println("dB: " + String(dB));

        return dB;
    }

}