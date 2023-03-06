# Installation Escape Game

## Node-RED

Rufe den Node-RED Workspace auf. Falls an der Installation von Node-RED nichts geändert wurde, läuft dieser über Port 1880. \
Klicke auf das Menü oben rechts und wähle Import aus. \
Klicke auf "Datei für Import auswählen" und wähle die Datei "escape-game_komplett.json" aus. Durch einen Klick auf Import wird alles eingefügt. \
Falls nun noch Module fehlen werden diese von Node-RED angezeigt. \
Über das Menü oben rechts und der Auswahl "Palette verwalten" können diese hinzugefügt werden. Dafür auf Installation klicken und die erforderten Module installieren.

## ZigBee2MQTT

Rufe die ZigBee2MQTT Website auf. Falls an der Installation nichts geändert wurde, läuft diese über Port 8080. \
Klicke auf "Anlernen aktivieren". Nun können alle ZigBee Geräte dem Netzwerk beitreten. \
Sind alle Geräte beigetreten, müssen sie noch in Node-RED eingefügt werden. Dafür müssen in allen Zigbee2MQTT Nodes die entsprechenden Geräte ausgewählt werden.

## MQTT

Falls nichts an Topics der über WLAN gesteuerten Geräte verändert wurde, kann dieser Schritt übersprungen werden. \
Sonst muss das Topic in den "MQTT-Out"-nodes in den Flows "Webpage" und "Rätsel 3"angepasst werden.

## Kiste

In der Datei "util.py" im Verzeichnis "e-Paper/src" muss die Variable "IP_KISTE" angepasst werden. Dieser muss die IP-Adresse der Kiste zugewiesen werden.
## Display

Falls nichts an dem Speicherort des Ordners "e-Paper" geändert wurde, kann dieser Schritt übersprungen werden. \
Sonst muss der Pfad zu den einzelnen Dateien in jeder "Pythonshell in"-node, jeder "exec"-node und in der "image speichern"-node angepasst werden.

## Fertigstellen

Klicke oben rechts auf die Schaltfläche **"Übernahme (deploy)"** \
Wenn alle Module installiert sind erscheint die Meldung "Erfolgreich übernommen (deploy)" 

Ob alle ZigBee2MQTT-Nodes sowie gegebenenfalls alle Pfade zum Display korrekt angepasst wurden, kann leider erst im Laufe des Spiels herausgefunden werden. Falls etwas vergessen wurde erscheint im "Debug"-Log eine Fehlermeldung. Der Debug-Log kann im rechten Reiter über die Schaltfläche mit dem Käfer aufgerufen werden. \
Falls ein MQTT-Topic vergessen wurde läuft der Code problemlos. Nur die über das Topic gesteuerten Geräte reagieren nicht. 

Ist alles Korrekt eingerichtet, muss noch die Sicherheitskopie aktualisiert werden. Dazu muss der gesamte Node-RED Code als JSON exportiert und unter dem Namen "flows_intia.json" in diesen Ordner gespeichert werden. Dabei wird die hier existierende Datei verworfen.  
Rufe dazu den Node-RED Workspace auf und klicke auf die drei Striche links oben. Mit Klick auf Exportieren öffnen sich ein ein Menü. Wähle in diesem die Schaltfläche "Alle Flows" aus und klicke dann auf Exportieren. Es öffnet sich ein Dateimanager, in welchem der Speicherort festgelegt wird. 


