import socket, sys, os, time, subprocess
from PIL import Image, ImageFont, ImageDraw

IP_KISTE = "192.168.1.101"

COLORED = 0
UNCOLORED = 1
WARTEN=30

MODULE_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'rpi_epd2in7')
if not MODULE_PATH in sys.path:
    sys.path.append(MODULE_PATH)

from rpi_epd2in7.epd import EPD
epd= EPD()

picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
font12 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 12)
font14 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 14)
font16 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 16)
font18 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 18)
font22 = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 22)



#Findet IP heraus
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

def initStatus(status, nummer, ip=None, image=None):
    if(not image):
       image=Image.new('1', (epd.height, epd.width), 255) 
    draw = ImageDraw.Draw(image)
    draw.line((0, 20, epd.height, 20), fill = COLORED)
    draw.line((0, 159, epd.height, 159), fill = COLORED)
    draw.text((1, 0), "Escape Game %(status)s - R\xE4tsel %(nummer)s - "%{"status": status, "nummer": nummer}, font = font12, fill = 0)
    if(ip):
        draw.text((1, 160), ip , font = font14, fill = COLORED)
    return image

def initRaetsel(image,nummer, untertitel, color):
    draw = ImageDraw.Draw(image)
    draw.text((36, epd.width/2-10),'R\xE4tsel %s:'%nummer, font = font18, fill = color)
    draw.text((36, epd.width/2+10), untertitel, font = font16, fill = color)
    return image


