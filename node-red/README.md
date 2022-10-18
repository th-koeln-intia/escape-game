# Installation Escape Game

## Node-RED

Rufe den Node-RED Workspace auf. Falls an der Installation von Node-RED nichts geändert wurde, läuft dieser über Port 1880. \
Klicke auf das Menü oben rechts und wähle Import aus. \
Klicke auf "Datei für Import auswählen" und wähle die Datei "escape-game_komplett.json" aus. Durch einen Klick auf Import wird alles eingefügt. \
Falls nun noch Module fehlen werden diese von Node-RED angezeigt./
Über das Menü oben rechts und der Auswahl "Palette verwalten" können diese hinzugefügt werden. Dafür auf Installation klicken und die erforderten Module installieren.

## ZigBee2MQTT

Rufe die ZigBee2MQTT Website auf. Falls an der Installation von Node-RED nichts geändert wurde, läuft diese über Port 8080. \
Klicke auf "Anlernen aktivieren". Nun können alle ZigBee Geräte dem Netzwerk beitreten. \
Sind alle Geräte beigetreten, müssen sie noch in Node-RED eingefügt werden. Dafür müssen in allen Zigbee2MQTT Nodes die entsprechenden Geräte ausgewählt werden.

## Display

Falls nichts an dem Speicherort des Ordners "e-Paper" geändert wurde, kann dieser Schritt übersprungen werden. \
Sonst muss der Pfad zu den einzelnen Dateien in jeder "Pythonshell in"-node, jeder "exec"-node und in den beiden "image speichern"-nodes angepasst werden.

## MQTT

Falls nichts an Topics der über WLAN gesteuerten Geräte verändert wurde, kann dieser Schritt übersprungen werden. \
Sonst muss das Topic in den "MQTT-Out"-nodes in den Flows "Webpage" und "Rätsel 3"angepasst werden.

## Kiste

In der Datei "util.py" im Verzeichnis "e-Paper/src" muss die Variable "IP_KISTE" angepasst werden. Dieser muss die IP-Adresse der Kiste zugewiesen werden.

## Fertigstellen

Klicke oben rechts auf die Schaltfläche **"Übernahme (deploy)"** \
Wenn alle Module installiert sind erscheint die Meldung "Erfolgreich übernommen (deploy)" 

Ob alle ZigBee2MQTT-Nodes sowie gegebenenfalls alle Pfade zum Display korrekt angepasst wurden, kann leider erst im Laufe des Spiels herausgefunden werden. Falls etwas vergessen wurde erscheint im "Debug"-Log eine Fehlermeldung. Der Debug-Log kann im rechten Reiter über die Schaltfläche mit dem Käfer aufgerufen werden. \
Falls ein MQTT-Topic vergessen wurde läuft der Code problemlos. Nur die über das Topic gesteuerten Geräte reagieren nicht.
