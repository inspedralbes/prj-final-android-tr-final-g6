#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <FS.h>
#include <WiFi.h>
#include <WebServer.h>

namespace accesspoint
{
    WebServer server(80); // mejor puerto 80 para AP local

    void saveWiFiConfig(const String& ssid, const String& password) {
        Serial.println("Guardando configuración WiFi a wifi.json...");
        StaticJsonDocument<256> jsonDoc;
        jsonDoc["ssid"] = ssid;
        jsonDoc["password"] = password;
        Serial.println("ssid");
        Serial.println(ssid);
        Serial.println("password");
        Serial.println(password);
        File configFile = SPIFFS.open("/wifi.json", "w");
        if (configFile) {
            configFile.print("{\"wifi_ssid\":\"" + ssid + "\",\"wifi_password\":\"" + password + "\"}");
            configFile.close();
        } else {
            Serial.println("Error abriendo wifi.json para escribir");
        }
    }

    void handleRoot() {
        File file = SPIFFS.open("/index.html", "r");
        if (!file) {
            server.send(500, "text/plain", "Error: no se encontró index.html");
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
            server.send(200, "text/plain", "Configuración guardada. Reinicia para aplicar.");
        } else {
            server.send(400, "text/plain", "Faltan SSID o password.");
        }
    }

    void handleRestart() {
        server.send(200, "text/plain", "Reiniciando...");
        delay(1000);
        ESP.restart();
    }

    void setupAccessPoint() {
        Serial.println("Iniciando punto de acceso...");
        if (WiFi.softAP("SENSORCILLO", "Jupiter1")) {
            Serial.println("Punto de acceso iniciado correctamente.");
        } else {
            Serial.println("Error al iniciar el punto de acceso.");
        }

        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP: ");
        Serial.println(IP.toString());

        server.on("/", handleRoot);
        server.on("/save", HTTP_POST, handleSave);
        server.on("/restart", HTTP_POST, handleRestart);
        server.begin();
        Serial.println("Servidor web iniciado.");
    }

    void loopAccessPoint() {
        server.handleClient();
    }

    bool isAccessPointActive() {
        return WiFi.softAPgetStationNum() > 0;
    }
}