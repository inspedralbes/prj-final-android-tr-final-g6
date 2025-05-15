#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <FS.h>
#include <WiFi.h>
#include <WebServer.h>

namespace accesspoint
{
    WebServer server(80); // millor port 80 per a AP local

    void saveWiFiConfig(const String& ssid, const String& password) {
        Serial.println("Guardant configuració WiFi a wifi.json...");
        StaticJsonDocument<256> jsonDoc;
        jsonDoc["ssid"] = ssid;
        jsonDoc["password"] = password;
        Serial.println("SSID:");
        Serial.println(ssid);
        Serial.println("Contrasenya:");
        Serial.println(password);
        File configFile = SPIFFS.open("/wifi.json", "w");
        if (configFile) {
            configFile.print("{\"wifi_ssid\":\"" + ssid + "\",\"wifi_password\":\"" + password + "\"}");
            configFile.close();
        } else {
            Serial.println("Error obrint wifi.json per escriure.");
        }
    }

    void handleRoot() {
        File file = SPIFFS.open("/index.html", "r");
        if (!file) {
            server.send(500, "text/plain", "Error: no s'ha trobat index.html");
            return;
        }

        String html = file.readString();
        server.send(200, "text/html", html);
        file.close();
    }

    void handleSave() {
        if (server.hasArg("ssid") && server.hasArg("password")) {
            String ssid = server.arg("ssid");
            String password = server.arg("password");
            saveWiFiConfig(ssid, password);
            server.send(200, "text/plain", "Configuració guardada. Reinicia per aplicar.");
        } else {
            server.send(400, "text/plain", "Falten SSID o contrasenya.");
        }
    }

    void handleRestart() {
        server.send(200, "text/plain", "Reiniciant...");
        delay(1000);
        ESP.restart();
    }

    void setupAccessPoint() {
        Serial.println("Iniciant punt d'accés...");
        if (WiFi.softAP("SENSORCILLO", "Jupiter1")) {
            Serial.println("Punt d'accés iniciat correctament.");
        } else {
            Serial.println("Error en iniciar el punt d'accés.");
        }

        IPAddress IP = WiFi.softAPIP();
        Serial.print("IP del punt d'accés: ");
        Serial.println(IP.toString());

        server.on("/", handleRoot);
        server.on("/save", HTTP_POST, handleSave);
        server.on("/restart", HTTP_POST, handleRestart);
        server.begin();
        Serial.println("Servidor web iniciat.");
    }

    void loopAccessPoint() {
        server.handleClient();
    }

    bool isAccessPointActive() {
        return WiFi.softAPgetStationNum() > 0;
    }
}