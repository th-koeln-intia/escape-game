import socket, sys, os
from PIL import Image, ImageFont, ImageDraw


# IP Adresse der Kiste, benötigt für Rätsel 1
IP_KISTE = "192.168.1.101"

# Wie lange das Titelbild für Rätsel angezeigt werden soll
WARTEN = 30

# Falls die Bibliothek zum Display nicht für Python auffindbar ist
MODULE_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'rpi_epd2in7')
if not MODULE_PATH in sys.path:
    sys.path.append(MODULE_PATH)

from rpi_epd2in7.epd import EPD

epd= EPD()

# Schriftgrößen
font12 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 12)
font14 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 14)
font16 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 16)
font18 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 18)
font22 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 22)

# Pfad zum Bildordner
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic') 

# Farben
COLORED = 0
UNCOLORED = 1



# Funktion zur ermittlung von IP Adresse
# return: IP Adresse
def getIP():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.connect(('10.255.255.255', 1))
        IP = sock.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        sock.close()
        return IP

# Funktion zum Initialisieren der Hintergrundbildes. Zeichnet Linien, Überschrift, Status des Escape Games und IP-Adresse
# Parameter: 
# status: "aktiv" oder "inaktiv", Zustand des Escape-Games
# number: Nummer des Rätsels
# ip: IP Adresse, falls schon ermittelt
# image: ein Imageobjekt auf das gemalt werden soll, falls keins übergeben wird, wird ein neues generiert
# return: Imageobjekt mit aktualisiertem Inhalt
def initStatus(status, number , ip=None, image=None):
    if(not image):
       image=Image.new('1', (epd.height, epd.width), 255) 
    draw = ImageDraw.Draw(image)
    draw.line((0, 20, epd.height, 20), fill = COLORED)
    draw.line((0, 159, epd.height, 159), fill = COLORED)
    draw.text((1, 0), "Escape Game %(status)s - R\xE4tsel %(number)s - "%{"status": status, "number": number}, font = font12, fill = COLORED)
    if(ip):
        draw.text((1, 160), ip , font = font14, fill = COLORED)
    return image

# Funktion zum Generieren des Titelbildes für die Rätsel. 
# Parameter:
# image: ein Imageobjekt auf das gemalt werden soll
# number: Nummer des Rätsels
# subtitle: Untertitel des Rätsels
# color: Farbe der Schrift
# return: Imageobjekt mit aktualisiertem Inhalt
def initRaetsel(image,number, subtitle, color):
    draw = ImageDraw.Draw(image)
    draw.text((36, epd.width/2-10),'R\xE4tsel %s:'%number, font = font18, fill = color)
    draw.text((36, epd.width/2+10), subtitle, font = font16, fill = color)
    return image


