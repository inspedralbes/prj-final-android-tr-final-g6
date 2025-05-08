#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "../config/config.h"

namespace connection {

String postNewSensor(String value) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    if (http.begin(client, config::url_newsensor)) {
        http.addHeader("Content-Type", "application/json");

        String key = "MAC";

        // Arma el JSON con clave y valor
        String payload = "{\"" + key + "\":\"" + value + "\"}";

        int httpCode = http.POST(payload);

        if (httpCode > 0) {
            // Si hay respuesta válida, obtenerla
            String response = http.getString();
            Serial.printf("POST OK, Código HTTP: %d\n", httpCode);
            http.end();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, response);

            if (error) {
                Serial.printf("Error al parsear JSON: %s\n", error.c_str());
                return "";
            }

            const char* apiKey = doc["apikey"];
            if (apiKey) {
                Serial.printf("API Key encontrada: %s\n", apiKey);
                return String(apiKey);
            } else {
                Serial.println("API Key no encontrada en la respuesta.");
                return "";
            }
            return response;
        } else {
            Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
            http.end();
            return "";
        }
    } else {
        Serial.println("No se pudo conectar al servidor.");
        return "";
    }
}

String postSensorData(String apikey, int volume, int temperature, int humidity, String MAC) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    if (http.begin(client, config::url_sensor)) {
        http.addHeader("Content-Type", "application/json");

        // Arma el JSON con los datos del sensor
        String payload = "{\"apikey\":\"" + apikey + "\",\"volume\":" + String(volume) +
                         ",\"temperature\":" + String(temperature) + 
                         ",\"humidity\":" + String(humidity) + 
                         ",\"MAC\":\"" + MAC + "\"}";

        int httpCode = http.POST(payload);

        if (httpCode > 0) {
            String response = http.getString();
            Serial.printf("POST OK, Código HTTP: %d\n", httpCode);
            http.end();
            return response;
        } else {
            Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
            http.end();
            return "";
        }
    } else {
        Serial.println("No se pudo conectar al servidor.");
        return "";
    }
}
}