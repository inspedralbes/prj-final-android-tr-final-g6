# Manual d'Instruccions: Canviar la Configuració WiFi del Dispositiu

Aquest manual explica com canviar la configuració WiFi del dispositiu utilitzant la seva interfície web.

---

## Passos per canviar la configuració WiFi

### 1. Accedir al punt d'accés del dispositiu
1. Si el dispositiu no està connectat a una xarxa WiFi, inicia el **Punt d'Accés** automàticament.
2. Busca la xarxa WiFi amb el nom (SSID): **SENSORCILLO**.
3. Connecta't a aquesta xarxa utilitzant la contrasenya: **Jupiter1**.

---

### 2. Obrir la interfície web
1. Un cop connectat al punt d'accés, obre un navegador web (Chrome, Firefox, etc.).
2. Introdueix la següent adreça a la barra de navegació:
http://192.168.4.1
3. Apareixerà la pàgina principal de configuració del dispositiu.

---

### 3. Introduir les noves credencials WiFi
1. A la pàgina principal, omple els camps següents:
- **SSID**: Introdueix el nom de la xarxa WiFi a la qual vols connectar el dispositiu.
- **Password**: Introdueix la contrasenya de la xarxa WiFi.
2. Fes clic al botó **Guardar**.

---

### 4. Reiniciar el dispositiu
1. Després de guardar la configuració, torneu a la web principal i fes clic al botó **Reiniciar**.
2. El dispositiu es reiniciarà i intentarà connectar-se a la xarxa WiFi amb les noves credencials.

---

## Resolució de problemes
- **El dispositiu no es connecta a la xarxa WiFi:**
  1. Verifica que el SSID i la contrasenya introduïts són correctes.
  2. Assegura't que la xarxa WiFi està activa i accessible.
  3. Si el problema persisteix, reinicia el dispositiu i torna a intentar-ho.

- **No pots accedir al punt d'accés:**
  1. Assegura't que el dispositiu està en mode **Punt d'Accés**.
  2. Reinicia el dispositiu i torna a intentar connectar-te.

---

## Notes addicionals
- Les credencials WiFi es guarden de manera segura al fitxer `wifi.json` al sistema de fitxers del dispositiu.
- Si necessites restablir la configuració de xarxa, pots esborrar el fitxer `wifi.json` manualment utilitzant un gestor de fitxers compatible amb SPIFFS.