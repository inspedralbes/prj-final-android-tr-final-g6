#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include "../../modules/config/config.h"
#include "../../modules/ldr/ldr.h"

namespace download {
    void downloadConfig() {
        Serial.println("Descarregant configuració des del servidor...");
        ldr::showImage("download.jpg");
        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;

        String url = "https://dev.acubox.cat/back/api/fileSensor/config.json";

        if (http.begin(client, url)) {
            int httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK) {
                File file = SPIFFS.open("/config.json", FILE_WRITE);
                if (!file) {
                    Serial.println("No s'ha pogut obrir el fitxer per escriure.");
                    http.end();
                    return;
                }

                http.writeToStream(&file);
                file.close();
                Serial.println("Configuració descarregada i desada com /config.json");
            } else {
                Serial.printf("Error en descarregar la configuració: Codi HTTP: %d\n", httpCode);
            }
            http.end();
        } else {
            Serial.println("No s'ha pogut connectar a la URL de configuració.");
        }
    }

    void downloadImages() {
        for (const String& image : config::images) {
            String url = image;
            String filename = String("/images/") + url.substring(url.lastIndexOf('/') + 1);

            Serial.printf("Descarregant imatge des de %s\n", url.c_str());

            WiFiClientSecure client;
            client.setInsecure();
            HTTPClient http;

            if (http.begin(client, url)) {
                int httpCode = http.GET();
                if (httpCode == HTTP_CODE_OK) {
                    File file = SPIFFS.open(filename, FILE_WRITE);
                    if (!file) {
                        Serial.printf("No s'ha pogut obrir el fitxer per escriure: %s\n", filename.c_str());
                        http.end();
                        continue;
                    }

                    http.writeToStream(&file);
                    file.close();
                    Serial.printf("Imatge desada: %s\n", filename.c_str());
                } else {
                    String payload = http.getString();
                    Serial.printf("Error en descarregar la imatge: %s, Codi HTTP: %d, Resposta: %s\n", url.c_str(), httpCode, payload.c_str());
                }
                http.end();
            } else {
                Serial.printf("No s'ha pogut connectar a la URL: %s\n", url.c_str());
            }
        }
    }
}