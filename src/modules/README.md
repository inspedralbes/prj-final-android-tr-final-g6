# Documentació dels Mòduls

Aquest document descriu els diferents mòduls que es troben a la carpeta `src/modules` del projecte. Cada mòdul té una funcionalitat específica que contribueix al funcionament general del sistema.

## Mòduls disponibles

### 1. AccesPoint
Aquest mòdul permet configurar un punt d'accés WiFi (Access Point) en un dispositiu ESP32, facilitant la configuració de la xarxa WiFi a través d'una interfície web senzilla.

#### Funcions principals

#### setupAccessPoint()

- Inicia el punt d'accés amb el SSID SENSORCILLO i la contrasenya Jupiter1.
- Mostra la IP assignada al punt d'accés.
- Configura les rutes del servidor web:
  - `/` per servir la pàgina principal (index.html).
  - `/save` per guardar la configuració WiFi (POST).
  - `/restart` per reiniciar el dispositiu (POST).
- Inicia el servidor web.

#### handleRoot()

- Serveix el fitxer index.html emmagatzemat a SPIFFS.
- Si el fitxer no existeix, retorna un error 500.

#### handleSave()

- Rep els paràmetres ssid i password des de la web.
- Crida a saveWiFiConfig() per guardar la configuració al fitxer wifi.json.
- Respon confirmant l'operació o indicant si falten paràmetres.

#### saveWiFiConfig(const String& ssid, const String& password)

- Guarda el SSID i la contrasenya en format JSON al fitxer wifi.json a SPIFFS.
- Mostra missatges pel port sèrie per a depuració.

#### handleRestart()

- Envia una resposta de confirmació i reinicia el dispositiu després d'un breu retard.

#### loopAccessPoint()

- Ha de ser cridat al bucle principal del programa per gestionar les peticions entrants al servidor web.

#### isAccessPointActive()

- Retorna true si hi ha algun dispositiu connectat al punt d'accés.

#### Resum AccesPoint

Aquest sistema permet configurar la xarxa WiFi del dispositiu de manera senzilla i segura, sense necessitat de connectar-lo prèviament a una xarxa coneguda. És ideal per a entorns on la configuració ha de ser dinàmica o accessible per a usuaris finals.

### 2. Config

El mòdul config gestiona la configuració general del dispositiu, incloent paràmetres de funcionament, credencials WiFi i la API Key, utilitzant fitxers emmagatzemats al sistema de fitxers SPIFFS.

#### Funcions principals

#### cargarConfig()

- Carrega la configuració general des del fitxer config.json.
- Si el fitxer no existeix, utilitza valors per defecte.
- Llegeix i assigna paràmetres com data, brillantor, llindars, URLs, imatges, etc.
- Mostra la configuració carregada pel port sèrie per a depuració.

#### cargarWifi()

- Carrega les credencials WiFi (wifi_ssid i wifi_password) des del fitxer wifi.json.
- Si el fitxer no existeix, manté els valors actuals o per defecte.

#### apikeyInsert(String apikey)

- Guarda la API Key al fitxer apikey.json en format JSON.
- Informa pel port sèrie si l'operació va ser exitosa o si hi va haver errors.

#### getApikey()

- Llegeix la API Key des del fitxer apikey.json.
- Retorna la clau si existeix, o una cadena buida si hi ha algun error.
- Informa pel port sèrie sobre l'estat de la lectura.

#### Variables principals

Paràmetres de configuració com:
- date, startglow, ldrThreshold, logoDelay, displayRotation, mostrarFractales, glowlevels, db_good, db_normal, db_angry, images, url_newsensor, url_sensor, vref_sound, maxSamples, etc.

Credencials WiFi:
- wifiSSID, wifiPassword

#### Resum de Config

Aquest mòdul centralitza la gestió de la configuració del dispositiu, permetent carregar i guardar paràmetres de funcionament, credencials WiFi i la API Key de manera persistent i segura. Facilita la personalització i el manteniment del sistema sense necessitat de recompilar el firmware.

### 3. Connection

El mòdul connection s'encarrega de la comunicació segura entre el dispositiu i el servidor remot, utilitzant peticions HTTP sobre TLS per registrar sensors i enviar dades de mesura.

#### Funcions principals

#### postNewSensor(String value)

- Envia una petició POST al servidor per registrar un nou sensor utilitzant la seva adreça MAC.
- Construeix un JSON amb la clau "MAC" i ho envia a l'endpoint configurat a config::url_newsensor.
- Si la resposta conté una clau "apiKey", l'extreu i la retorna.
- Si ocorre algun error o la clau no està present, retorna "EXAMPLE" com a valor per defecte.

#### postSensorData(String apikey, int volume, int temperature, int humidity, String dateTime, String MAC)

- Envia una petició POST al servidor amb les dades del sensor: volum, temperatura, humitat, data i MAC, juntament amb la API Key.
- Utilitza l'endpoint configurat a config::url_sensor.
- Analitza la resposta del servidor:
  - Si la resposta conté una data ("date") més recent que l'emmagatzemada localment, descarrega i actualitza la configuració i les imatges.
  - Si la data és més antiga, no realitza canvis.
  - Si ocorre un error en la petició, intenta registrar el sensor de nou cridant a postNewSensor.
- Retorna la resposta del servidor com a cadena.

#### Resum

Aquest mòdul permet al dispositiu:

- Registrar-se automàticament al servidor la primera vegada que es connecta.
- Enviar periòdicament les dades dels sensors de manera segura.
- Mantenir la configuració actualitzada si el servidor indica que hi ha una versió més recent.

La comunicació es realitza sempre de manera segura (HTTPS) i el maneig d'errors està orientat a la robustesa i l'autogestió del dispositiu.

### 4. Download

El mòdul download permet descarregar la configuració i les imatges des del servidor remot i guardar-les al sistema de fitxers SPIFFS.

#### Funcions principals

#### downloadConfig()

- Descarrega el fitxer de configuració `config.json` des d'una URL predefinida.
- Desa el fitxer descarregat a SPIFFS com `/config.json`.
- Mostra missatges pel port sèrie sobre l'estat de la descàrrega.

#### downloadImages()

- Descarrega totes les imatges especificades a la configuració (`config::images`).
- Desa cada imatge a la carpeta `/images/` de SPIFFS.
- Informa pel port sèrie de l'estat de cada descàrrega.

#### Resum Download

Aquest mòdul facilita la gestió remota de recursos, permetent actualitzar la configuració i les imatges sense necessitat de reprogramar el dispositiu.

---

### 5. LDR

El mòdul ldr gestiona la lectura del sensor de llum (LDR) i el control del nivell de brillantor del dispositiu.

#### Funcions principals

#### setup()

- Inicialitza el pin del sensor LDR.

#### read()

- Llegeix el valor analògic del sensor de llum.

#### checkChange()

- Detecta canvis en la il·luminació per modificar el nivell de brillantor.

#### getBrightnessLevel()

- Retorna el nivell de brillantor actual segons la configuració.

#### showImage(const char *filename)

- Mostra una imatge a la pantalla a partir del fitxer especificat.

#### Resum LDR

Permet adaptar la brillantor del dispositiu segons la llum ambiental i mostrar imatges en funció del nivell detectat.

---

### 6. Sound

El mòdul sound permet llegir el nivell de so ambiental mitjançant un sensor de micròfon.

#### Funcions principals

#### initSoundSensor()

- Inicialitza el pin del micròfon.

#### readSoundDataInDecibels()

- Llegeix i calcula el nivell de so en decibels (dB).
- Utilitza la referència `config::vref_sound` per a la conversió.

#### Resum Sound

Permet monitoritzar el soroll ambiental i adaptar el comportament del dispositiu segons el nivell de so detectat.

---

### 7. Humidity

El mòdul humidity gestiona la lectura del sensor d'humitat mitjançant I2C.

#### Funcions principals

#### init()

- Inicialitza el sensor d'humitat.

#### readHumidity()

- Llegeix el percentatge d'humitat relativa des del sensor.

#### Resum Humidity

Permet obtenir la humitat ambiental per a la seva utilització en la lògica del dispositiu o per enviar-la al servidor.

---

### 8. Temperature

El mòdul temperature permet llegir la temperatura ambiental mitjançant el sensor HDC1080.

#### Funcions principals

#### readTemperatureSensor()

- Llegeix la temperatura en graus Celsius des del sensor via I2C.

#### Resum Temperature

Proporciona la temperatura ambiental per a la seva visualització o enviament al servidor.

---

### 9. WiFi

El mòdul wifi gestiona la connexió del dispositiu a la xarxa WiFi utilitzant les credencials carregades.

#### Funcions principals

#### connectToWiFi()

- Intenta connectar-se a la xarxa WiFi utilitzant el SSID i la contrasenya de la configuració.
- Mostra l'estat de la connexió pel port sèrie.

#### Resum WiFi

Permet establir la connexió a Internet necessària per a la comunicació amb el servidor i la descàrrega de recursos.

---

### 10. Emojis

El mòdul emojis pot contenir funcionalitats per mostrar o gestionar emojis o icones gràfiques al dispositiu.

#### Funcions principals

- (Consultar el codi per a funcionalitats específiques.)

#### Resum Emojis

Permet enriquir la interfície gràfica del dispositiu amb icones o emojis.

---

## Estructura general dels fitxers

- **.h**: Defineix les funcions i variables externes del mòdul.
- **.cpp**: Implementa la lògica de les funcions declarades a la capçalera.

## Recomanacions d'ús

- Consulta aquest document per entendre la funcionalitat de cada mòdul abans de modificar o ampliar el codi.
- Mantingues la separació de responsabilitats entre mòduls per facilitar el manteniment i la reutilització del codi.
- Utilitza el sistema de fitxers SPIFFS per a la configuració persistent i la gestió de recursos (fitxers JSON, HTML, etc.).

## Resum general

Aquest projecte està dissenyat per ser modular, flexible i fàcilment configurable. Cada mòdul aporta una funcionalitat concreta i la seva documentació facilita la comprensió i l'extensió del sistema per a futures millores o adaptacions.

---