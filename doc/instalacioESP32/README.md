# Guia d'Instal·lació del Firmware al ESP32

Aquest document explica com instal·lar el firmware al dispositiu ESP32 i configurar la xarxa WiFi.

## Requisits previs

### Hardware
- **ESP32**
- Cable USB compatible amb el ESP32
- Ordinador amb port USB disponible

### Software
- [Visual Studio Code](https://code.visualstudio.com/) amb l'extensió **PlatformIO** instal·lada
- Drivers USB per al ESP32 (si és necessari)
- Llibreries requerides (es defineixen al fitxer `platformio.ini`)

## Passos per instal·lar el firmware

### 1. Clonar el repositori
1. Obre un terminal al teu ordinador.
2. Clona el repositori del projecte:
    ```bash
    git clone <URL-del-repositori>
    ```

### 2. Obre el projecte a Visual Studio Code
Obre Visual Studio Code i carrega el projecte clonat.

### 3. Configurar la xarxa WiFi
A la carpeta `data/`, crea un fitxer anomenat `wifi.json` si no existeix.

Obre el fitxer `wifi.json` i introdueix les credencials de la xarxa WiFi:

```json
{
  "wifi_ssid": "NomDeLaTevaXarxa",
  "wifi_password": "ContrasenyaDeLaTevaXarxa"
}
```

Desa el fitxer.

### 4. Carregar els fitxers al sistema SPIFFS
A PlatformIO, selecciona l'opció **Upload File System Image** per carregar els fitxers de la carpeta `data/` al sistema de fitxers del ESP32.

Espera que el procés finalitzi correctament.

### 5. Compilar i carregar el firmware
Connecta l'ESP32 al teu ordinador mitjançant el cable USB.

A PlatformIO, selecciona l'opció **Upload** per compilar i carregar el firmware al ESP32.

Espera que el procés finalitzi. Si tot és correcte, veuràs un missatge indicant que la càrrega ha estat exitosa.

### 6. Verificar la instal·lació
Obre el monitor sèrie a PlatformIO seleccionant l'opció **Monitor**.

Observa els missatges al monitor sèrie per verificar que el dispositiu s'ha connectat correctament a la xarxa WiFi i que el firmware està funcionant.

## Resolució de problemes

### El dispositiu no es connecta a la xarxa WiFi:
- Verifica que el fitxer `wifi.json` conté les credencials correctes.
- Assegura't que la xarxa WiFi està activa i accessible.
- Torna a carregar els fitxers SPIFFS i reinicia el dispositiu.

### Error en carregar el firmware:
- Verifica que l'ESP32 està correctament connectat al port USB.
### Enllaç a la guia de configuració
- Per a més informació sobre la configuració del dispositiu, consulta el [README de Configuració](../ConfiguracioWifi/README.md).
- Si necessites restablir la configuració, pots esborrar el fitxer `wifi.json` i tornar a carregar-lo.