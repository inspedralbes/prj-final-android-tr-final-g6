#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "../config/config.h"
#include "../download/download.h"
#include <ArduinoJson.h>

namespace connection
{

    String postNewSensor(String value)
    {
        Serial.println("Iniciando postNewSensor...");
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient http;
        if (http.begin(client, config::url_newsensor))
        {
            Serial.println("Conexión al servidor exitosa para postNewSensor.");
            http.addHeader("Content-Type", "application/json");

            // Construye el JSON con la clave y el valor
            String payload = "{\"MAC\":\"" + value + "\"}";
            Serial.print("Payload enviado: ");
            Serial.println(payload);

            int httpCode = http.POST(payload);

            if (httpCode > 0)
            {
                String response = http.getString();
                Serial.printf("POST OK, Código HTTP: %d\n", httpCode);
                Serial.print("Respuesta del servidor: ");
                Serial.println(response);

                http.end();

                // Analizar JSON con ArduinoJson
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, response);

                if (error)
                {
                    Serial.print("Error al analizar JSON: ");
                    Serial.println(error.c_str());
                    return "";
                }

                // Obtener la API Key
                Serial.println("Extrayendo apiKey...");
                const char *apiKey = doc["apiKey"];
                if (apiKey)
                {
                    Serial.print("API Key extraída: ");
                    Serial.println(apiKey);
                    return String(apiKey);
                }
                else
                {
                    Serial.println("No se encontró 'apiKey' en la respuesta.");
                    return "";
                }
            }
            else
            {
                Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
                http.end();
                return "";
            }
        }
        else
        {
            Serial.println("No se pudo conectar al servidor en postNewSensor.");
            return "";
        }
    }

    String postSensorData(String apikey, int volume, int temperature, int humidity, String dateTime, String MAC)
    {
        Serial.println("Iniciando postSensorData...");
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient http;
        // Serial.println("Conectando al servidor...");
        Serial.println(config::url_sensor);
        if (http.begin(client, config::url_sensor))
        {
            // Serial.println("Conexión al servidor exitosa para postSensorData.");
            http.addHeader("Content-Type", "application/json");

            // Arma el JSON con los datos del sensor
            String payload = "{\"api_key\":\"" + apikey + "\",\"volume\":" + String(volume) +
                             ",\"temperature\":" + String(temperature) +
                             ",\"humidity\":" + String(humidity) +
                             ",\"date\":\"" + dateTime + "\"" +
                             ",\"MAC\":\"" + MAC + "\"}";
            Serial.print("Payload enviado: ");
            Serial.println(payload);

            int httpCode = http.POST(payload);

            if (httpCode > 0)
            {
                String response = http.getString();
                // Serial.printf("POST OK, Código HTTP: %d\n", httpCode);
                // Serial.print("Respuesta del servidor: ");
                // Serial.println(response);

                // Analizar JSON con ArduinoJson
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, response);

                if (error)
                {
                    Serial.print("Error al analizar JSON: ");
                    Serial.println(error.c_str());
                    return "";
                }

                // Comparar la fecha del JSON con la fecha del config.json
                const char *responseDate = doc["date"];
                if (responseDate)
                {
                    // Serial.print("Fecha recibida del servidor: ");
                    Serial.println(responseDate);

                    if (String(responseDate) > config::date)
                    {
                        Serial.println("La fecha recibida es más reciente que la del config.json.");
                        Serial.println("Actualizando fecha en config.json...");
                        download::downloadConfig();
                        config::cargarConfig();
                        download::downloadImages();
                    }
                    else
                    {
                        Serial.println("La fecha recibida es más antigua que la del config.json.");
                    }
                }
                else
                {
                    Serial.println("No se encontró 'date' en la respuesta.");
                }
                
                http.end();
                return response;
            }
            else
            {
                Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
                Serial.println("Intentando registrar nuevo sensor...");
                connection::postNewSensor(MAC);
                http.end();
                return "";
            }
        }
        else
        {
            Serial.println("No se pudo conectar al servidor en postSensorData.");
            return "";
        }
    }
}