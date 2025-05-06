#include "humidity.h"
#include <Wire.h>
#include "pins.h"

namespace humidity {
    const uint8_t HDC1080_ADDRESS = 0x40; // Adreça I2C del sensor HDC1080

    void init() {
        Wire.begin(Pines::HDC1080_SDA, Pines::HDC1080_SCL);
        Wire.beginTransmission(HDC1080_ADDRESS);
        Wire.write(0x02); // Registre de configuració
        Wire.write(0x10); // Configura per mesurar humitat
        Wire.endTransmission();
        delay(15); // Espera que el sensor estigui llest
    }

    float readHumidity() {
        Wire.beginTransmission(HDC1080_ADDRESS);
        Wire.write(0x01); // Registre de humitat
        Wire.endTransmission();
        delay(15); // Espera la conversió

        Wire.requestFrom(HDC1080_ADDRESS, 2);
        if (Wire.available() == 2) {
            uint16_t rawHumidity = (Wire.read() << 8) | Wire.read();
            return (rawHumidity / 65536.0) * 100.0; // Converteix a percentatge
        }
        return -1.0; // Retorna -1 si hi ha error
    }
}