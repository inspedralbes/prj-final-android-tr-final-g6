# Acoubox Sensor

## Nom dels integrants
- Xavier Mateu Martínez
- Martí Clavé
- Fabián Roldán
- Climent Fernández

## Nom del projecte
**Acoubox**

## Petita descripció
Aquest projecte planteja el desenvolupament d’un dispositiu capaç de recopilar dades relacionades amb l’acústica, els nivells de humitat i el volum de les aules d’un institut. La informació recollida es processarà per generar gràfics que facilitin l’anàlisi de les condicions ambientals i acústiques dels espais educatius. L’objectiu és proporcionar eines visuals per avaluar i optimitzar el confort i la sostenibilitat a les aules, promovent entorns d’aprenentatge més saludables. La iniciativa combina l’ús de sensors avançats, l’anàlisi de dades i la visualització gràfica per a una gestió eficient dels recursos educatius.

## Característiques principals
- **Sensors utilitzats**: acústica, humitat i temperatura.
- **Plataforma de maquinari**: ESP32.
- **Processament de dades**: Generació de gràfics per a l'anàlisi.
- **Objectiu**: Millorar el confort i la sostenibilitat en espais educatius.

## Requisits del projecte
- **Hardware**:
  - ESP32
  - Sensor d'humitat
  - Sensor de temperatura
  - Micròfon
  - Altaveu
  - LCD
- **Software**:
  - [PlatformIO](https://platformio.org/) per a la gestió del projecte
  - Llibreries utilitzades:

    - **NTPClient** (versió 3.2.1): Llibreria per gestionar el protocol de temps de xarxa (NTP).  
        [Enllaç a la llibreria](https://platformio.org/lib/show/551/NTPClient)

    - **TJpg_Decoder** (versió 1.1.0): Llibreria per a la decodificació de fitxers JPEG en pantalles gràfiques.  
        [Enllaç a la llibreria](https://platformio.org/lib/show/6593/TJpg_Decoder)

    - **ArduinoJson** (versió 7.4.1): Llibreria per treballar amb dades JSON de manera eficient.  
        [Enllaç a la llibreria](https://platformio.org/lib/show/64/ArduinoJson)

    - **AnimatedGIF** (versió 2.2.0): Llibreria per renderitzar fitxers GIF animats en pantalles gràfiques.  
        [Enllaç a la llibreria](https://platformio.org/lib/show/1851/AnimatedGIF)

## Manuals
  - [Configuració del Wifi](doc/configuracioWifi.md)
  - [Instalació del firmware al ESP32](doc/instalacioESP32.md)