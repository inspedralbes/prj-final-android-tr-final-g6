#include "pins.h"
#include <Wire.h>
#include <Arduino.h>

namespace temperature {
    float readTemperatureSensor() {
        // Inicializar I2C
        Wire.begin(Pines::HDC1080_SDA, Pines::HDC1080_SCL);

        // Dirección del sensor HDC1080
        const uint8_t HDC1080_ADDRESS = 0x40;

        // Enviar comando para leer temperatura (registro 0x00)
        Wire.beginTransmission(HDC1080_ADDRESS);
        Wire.write(0x00); // Registro de temperatura
        Wire.endTransmission();

        // Esperar a que el sensor procese la lectura
        delay(15);

        // Leer 2 bytes de datos de temperatura
        Wire.requestFrom(HDC1080_ADDRESS, 2);
        if (Wire.available() < 2) {
            Serial.println("⚠️ Error al leer datos del sensor HDC1080.");
            return -999.0; // Valor de error
        }

        uint16_t rawTemperature = (Wire.read() << 8) | Wire.read();

        // Convertir la lectura a grados Celsius
        float temperature = (rawTemperature / 65536.0) * 165.0 - 40.0;

        return temperature;
    }
}