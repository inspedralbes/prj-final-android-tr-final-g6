# Documentació dels Mòduls

Aquest document descriu els diferents mòduls que es troben a la carpeta `src/modules` del projecte. Cada mòdul té una funcionalitat específica que contribueix al funcionament general del sistema.

## Mòduls disponibles

### 1. **SensorModule**
- **Descripció**: Aquest mòdul s'encarrega de gestionar la lectura de dades dels sensors connectats al dispositiu. Inclou funcionalitats per inicialitzar els sensors, llegir valors i processar les dades.
- **Funcions principals**:
  - `initSensors()`: Inicialitza tots els sensors.
  - `readSensorData()`: Llegeix les dades dels sensors i les retorna en format JSON.
- **Dependències**:
  - Llibreries de sensors específiques (per exemple, DHT, BMP, etc.).

---

### 2. **WiFiModule**
- **Descripció**: Gestiona la connexió WiFi del dispositiu. Inclou funcionalitats per connectar-se a una xarxa, gestionar credencials i verificar l'estat de la connexió.
- **Funcions principals**:
  - `connectToWiFi(ssid, password)`: Connecta el dispositiu a una xarxa WiFi.
  - `isConnected()`: Retorna l'estat de la connexió.
- **Dependències**:
  - Llibreria `WiFi.h` per a la gestió de xarxes.

---

### 3. **DataProcessingModule**
- **Descripció**: Aquest mòdul processa les dades recollides pels sensors. Inclou funcionalitats per analitzar, filtrar i preparar les dades per a la seva visualització o enviament.
- **Funcions principals**:
  - `filterNoise(data)`: Filtra el soroll de les dades recollides.
  - `prepareGraphData(data)`: Prepara les dades per generar gràfics.
- **Dependències**:
  - Llibreria `ArduinoJson` per al maneig de dades JSON.

---

### 4. **DisplayModule**
- **Descripció**: Gestiona la interfície gràfica del dispositiu. Aquest mòdul controla la pantalla i mostra informació com els valors dels sensors, gràfics i notificacions.
- **Funcions principals**:
  - `initDisplay()`: Inicialitza la pantalla.
  - `updateDisplay(data)`: Actualitza la informació mostrada a la pantalla.
- **Dependències**:
  - Llibreria `Adafruit_GFX` i `TJpg_Decoder`.

---

### 5. **CommunicationModule**
- **Descripció**: Gestiona la comunicació amb servidors externs o altres dispositius. Inclou funcionalitats per enviar dades a un servidor o rebre configuracions.
- **Funcions principals**:
  - `sendDataToServer(data)`: Envia dades al servidor configurat.
  - `receiveConfig()`: Rep configuracions des d'un servidor.
- **Dependències**:
  - Llibreria `HTTPClient` per a peticions HTTP.

---

### 6. **UtilsModule**
- **Descripció**: Conté funcions auxiliars i utilitats que són utilitzades per altres mòduls. Aquest mòdul ajuda a evitar la duplicació de codi.
- **Funcions principals**:
  - `formatDate(timestamp)`: Converteix un timestamp en una data llegible.
  - `calculateAverage(values)`: Calcula la mitjana d'una llista de valors.
- **Dependències**:
  - Cap dependència específica.