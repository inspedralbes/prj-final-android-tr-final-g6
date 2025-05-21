# Documentació del fitxer `main.cpp`

## Contingut del fitxer

### 1. **Variables Globals**
- **Pantalla LED**:
  - `MatrixPanel_I2S_DMA *dma_display`: Objecte per controlar la pantalla LED.
- **Estat del dispositiu**:
  - `bool connected`: Indica si el dispositiu està connectat a WiFi.
  - `bool apActive`: Indica si el punt d'accés està actiu.
  - `char dateTime[20]`: Per emmagatzemar la data i hora actual.
  - `String apikey`: Clau API per comunicar-se amb el servidor.
- **Control de visualització**:
  - `bool clockDisplayActive`: Indica si el rellotge està actiu.
  - `unsigned long clockDisplayStart`: Temps d'inici de la visualització del rellotge.

### 2. **Funcions Principals**

#### `bool tjpgDrawCallback(...)`
Callback per dibuixar imatges JPEG a la pantalla LED.

#### `void displaySetup(uint8_t displayBright, uint8_t displayRotation)`
Configura la pantalla LED amb la brillantor i rotació especificades.

#### `void setup()`
Funció d'inicialització:
1. Configura el port sèrie i els pins.
2. Carrega la configuració des de SPIFFS.
3. Configura la pantalla LED.
4. Intenta connectar-se a la xarxa WiFi:
   - Si té èxit, descarrega configuracions i imatges.
   - Si falla, crea un punt d'accés WiFi.
5. Inicialitza els sensors i obté la clau API.

#### `void loop()`
Funció principal que s'executa contínuament:
1. Verifica la connexió WiFi i intenta reconnectar si és necessari.
2. Ajusta la brillantor de la pantalla segons el sensor LDR.
3. Mostra emojis basats en els nivells de so i temperatura.
4. Envia dades al servidor cada segon.
5. Mostra el rellotge cada minut durant 5 segons.

### 3. **Flux de Treball**

1. **Configuració Inicial**:
   - Carrega configuracions des de SPIFFS.
   - Configura la pantalla LED i els sensors.
   - Intenta connectar-se a la xarxa WiFi o crea un punt d'accés.

2. **Lectura de Sensors**:
   - Llegeix dades dels sensors de temperatura, so i humitat.
   - Calcula mitjanes per evitar valors erràtics.

3. **Visualització**:
   - Mostra emojis basats en els nivells de so.
   - Mostra el rellotge cada minut.

4. **Comunicació amb el Servidor**:
   - Envia dades dels sensors al servidor.
   - Actualitza la clau API si és necessari.

### 4. **Mòduls Utilitzats**

Pots veure mes informacio al [README de Mòduls](./modules/README.md)


### 5. **Resolució de Problemes**

- **No es connecta a WiFi**:
  - Verifica les credencials WiFi al fitxer `wifi.json`.
  - Assegura't que la xarxa WiFi està activa.

- **Error en la visualització**:
  - Verifica que la pantalla LED està correctament connectada.
  - Revisa els pins configurats al mòdul `pins.h`.

- **No envia dades al servidor**:
  - Verifica que la clau API és vàlida.
  - Assegura't que el servidor està actiu i accessible.