#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "../config/config.h"
#include "../download/download.h"
#include <ArduinoJson.h>

namespace connection
{

    String postNewSensor(String value)
    {
        Serial.println("Iniciant postNewSensor...");
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient http;
        if (http.begin(client, config::url_newsensor))
        {
            Serial.println("Connexió al servidor exitosa per a postNewSensor.");
            http.addHeader("Content-Type", "application/json");

            // Construeix el JSON amb la clau i el valor
            String payload = "{\"MAC\":\"" + value + "\"}";
            Serial.print("Payload enviat: ");
            Serial.println(payload);

            int httpCode = http.POST(payload);

            if (httpCode > 0)
            {
                String response = http.getString();
                Serial.printf("POST OK, Codi HTTP: %d\n", httpCode);

                http.end();

                // Analitzar JSON amb ArduinoJson
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, response);

                if (error)
                {
                    Serial.print("Error en analitzar JSON: ");
                    Serial.println(error.c_str());
                    return "EXAMPLE";
                }

                // Obtenir l'API Key
                Serial.println("Extraient apiKey...");
                const char *apiKey = doc["apiKey"];
                if (apiKey)
                {
                    return String(apiKey);
                }
                else
                {
                    Serial.println("No s'ha trobat 'apiKey' a la resposta.");
                    return "EXAMPLE";
                }
            }
            else
            {
                Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
                http.end();
                return "EXAMPLE";
            }
        }
        else
        {
            Serial.println("No s'ha pogut connectar al servidor en postNewSensor.");
                return "EXAMPLE";
        }
    }

    String postSensorData(String apikey, int volume, int temperature, int humidity, String dateTime, String MAC)
    {
        Serial.println("Iniciant postSensorData...");
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient http;
        Serial.println(config::url_sensor);
        if (http.begin(client, config::url_sensor))
        {
            http.addHeader("Content-Type", "application/json");

            // Construeix el JSON amb les dades del sensor
            String payload = "{\"api_key\":\"" + apikey + "\",\"volume\":" + String(volume) +
                             ",\"temperature\":" + String(temperature) +
                             ",\"humidity\":" + String(humidity) +
                             ",\"date\":\"" + dateTime + "\"" +
                             ",\"MAC\":\"" + MAC + "\"}";
            
            Serial.print("Payload enviat.");

            int httpCode = http.POST(payload);

            if (httpCode > 0)
            {
                String response = http.getString();

                // Analitzar JSON amb ArduinoJson
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, response);

                if (error)
                {
                    Serial.print("Error en analitzar JSON: ");
                    Serial.println(error.c_str());
                    return "";
                }

                // Comparar la data del JSON amb la data del config.json
                const char *responseDate = doc["date"];
                if (responseDate)
                {
                    Serial.println(responseDate);

                    if (String(responseDate) > config::date)
                    {
                        Serial.println("La data rebuda és més recent que la del config.json.");
                        Serial.println("Actualitzant data al config.json...");
                        download::downloadConfig();
                        config::cargarConfig();
                        download::downloadImages();
                    }
                    else
                    {
                        Serial.println("La data rebuda és més antiga que la del config.json.");
                    }
                }
                else
                {
                    Serial.println("No s'ha trobat 'date' a la resposta.");
                }
                
                http.end();
                return response;
            }
            else
            {
                Serial.printf("Error en POST: %s\n", http.errorToString(httpCode).c_str());
                Serial.println("Intentant registrar un nou sensor...");
                connection::postNewSensor(MAC);
                http.end();
                return "";
            }
        }
        else
        {
            Serial.println("No s'ha pogut connectar al servidor en postSensorData.");
            return "";
        }
    }
}
