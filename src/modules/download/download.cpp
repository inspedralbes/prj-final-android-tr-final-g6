#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include "../../modules/config/config.h"

namespace download {
    void downloadConfig() {
        Serial.println("Descargando configuración desde el servidor...");

        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;

        String url = "https://dev.acubox.cat/back/api/fileSensor/config.json";

        if (http.begin(client, url)) {
            int httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK) {
                File file = SPIFFS.open("/config.json", FILE_WRITE);
                if (!file) {
                    Serial.println("No se pudo abrir el archivo para escribir.");
                    http.end();
                    return;
                }

                http.writeToStream(&file);
                file.close();
                Serial.println("Configuración descargada y guardada como /config.json");
            } else {
                Serial.printf("Error al descargar la configuración: Código HTTP: %d\n", httpCode);
            }
            http.end();
        } else {
            Serial.println("No se pudo conectar a la URL de configuración.");
        }
    }

    void downloadImages() {
        for (const String& image : config::images) {
            String url = image;
            String filename = String("/images/") + url.substring(url.lastIndexOf('/') + 1);

            Serial.printf("Descargando imagen desde %s\n", url.c_str());

            WiFiClientSecure client;
            client.setInsecure();
            HTTPClient http;

            if (http.begin(client, url)) {
                int httpCode = http.GET();
                if (httpCode == HTTP_CODE_OK) {
                    File file = SPIFFS.open(filename, FILE_WRITE);
                    if (!file) {
                        Serial.printf("No se pudo abrir el archivo para escribir: %s\n", filename.c_str());
                        http.end();
                        continue;
                    }

                    http.writeToStream(&file);
                    file.close();
                    Serial.printf("Imagen guardada: %s\n", filename.c_str());
                } else {
                    String payload = http.getString();
                    Serial.printf("Error al descargar la imagen: %s, Código HTTP: %d, Respuesta: %s\n", url.c_str(), httpCode, payload.c_str());
                }
                http.end();
            } else {
                Serial.printf("No se pudo conectar a la URL: %s\n", url.c_str());
            }
        }
    }
}