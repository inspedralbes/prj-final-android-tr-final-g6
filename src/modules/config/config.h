#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace config {
    extern String date;
    extern int startglow;
    extern int ldrThreshold;
    extern int logoDelay;
    extern int displayRotation;
    extern String wifiSSID;
    extern String wifiPassword;
    extern bool mostrarFractales;
    extern int glowlevels[5];
    extern int numNiveles;
    extern int db_good;
    extern int db_normal;
    extern int db_angry;
    extern String images[10];
    extern String url_newsensor;
    extern String url_sensor;
    extern float vref_sound;

    void cargarConfig();
    void cargarWifi();

    void apikeyInsert(String apikey);
    String getApikey();
}

#endif
