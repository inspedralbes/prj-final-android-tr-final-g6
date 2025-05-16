#include "pins.h"
#include <Wire.h>
#include <Arduino.h>

namespace temperature {
    float readTemperatureSensor() {
        // Inicialitzar I2C
        Wire.begin(Pines::HDC1080_SDA, Pines::HDC1080_SCL);

        // Adreça del sensor HDC1080
        const uint8_t HDC1080_ADDRESS = 0x40;

        // Enviar comanda per llegir temperatura (registre 0x00)
        Wire.beginTransmission(HDC1080_ADDRESS);
        Wire.write(0x00); // Registre de temperatura
        Wire.endTransmission();

        // Esperar que el sensor processi la lectura
        delay(15);

        // Llegir 2 bytes de dades de temperatura
        Wire.requestFrom(HDC1080_ADDRESS, 2);
        if (Wire.available() < 2) {
            Serial.println("⚠️ Error en llegir dades del sensor HDC1080.");
            return -999.0; // Valor d'error
        }

        uint16_t rawTemperature = (Wire.read() << 8) | Wire.read();

        // Convertir la lectura a graus Celsius
        float temperature = (rawTemperature / 65536.0) * 165.0 - 40.0;

        return temperature;
    }
}