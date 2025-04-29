#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Declaración de variables de configuración
extern int brilloInicial;
extern int ldrThreshold;
extern int displayRotation;
extern int fractalDelay;
extern int logoDelay;
extern String wifiSSID;
extern String wifiPassword;
extern bool mostrarFractales;
extern int nivelesBrillo[5];  // ejemplo para 5 niveles
extern int numNiveles;

// Función para cargar config desde JSON
void cargarConfig();

#endif
