# -*- coding: iso-8859-1 -*-

from util import *

def main():
    epd.init()  
    # Am Anfang sind weder Netzwerk noch Kiste verfügbar
    kiste = False
    netzwerk = False
    # IP Adresse und Linien
    image = initStatus("Inaktiv", "1")
    draw = ImageDraw.Draw(image)
    epd.display_frame(image.rotate(90, expand=True))
    image=initRaetsel(image,'1', 'Buchstaben-Salat', COLORED)
    epd.display_partial_frame(image.rotate(90, expand=True), 0, 0, epd.height, epd.width, fast=True)
    time.sleep(WARTEN)
    for _ in range(20):
        IP=getIP()     
        if(IP.startswith('192')):
            netzwerk = True
            if( subprocess.call(["ping", "-c 1", IP_KISTE], stdout=subprocess.DEVNULL,shell=False)==0):
                kiste = True
                image = initStatus("Inaktiv", "1", IP)
                draw = ImageDraw.Draw(image)
                draw.text((64, epd.width/2), 'System bereit' , font = font16, fill = COLORED)
                epd.display_frame(image.rotate(90, expand=True))
                break
        time.sleep(10)

    #Wenn nach 5 Minuten kein Netzwerk gefunden wurde wird abgebrochen
    if(netzwerk and kiste):
        print( "bereit")
    elif (not netzwerk):
        image=initRaetsel(image,'1', 'Buchstaben-Salat', UNCOLORED)
        draw.text((16, (epd.width/2)-36), 'Falsche Reihenfolge' , font = font18, fill = COLORED)
        draw.text((16, (epd.width/2)-16), 'Bitte schlie\xdft alles neu an!' , font = font16, fill = COLORED)
        draw.text((16, (epd.width/2)), 'Tipp: 1. Mini-Computer \x0a2. Router 3. Kiste' , font = font16, fill = COLORED)
        draw.text((1, 160), "FEHLER", font = font14, fill = COLORED)
        epd.display_partial_frame(image.rotate(90, expand=True), 0, 0, epd.height, epd.width, fast=True)
        print("fehler")
    
    elif(not kiste):
        image=initRaetsel(image,'1', 'Buchstaben-Salat', UNCOLORED)
        draw.text((10, (epd.width/2)-36), 'Kiste nicht verbunden:' , font = font18, fill = COLORED)
        draw.text((10, (epd.width/2)-16), 'schlie\xdft die Kiste neu an\x0aund dr\xfcckt die zweite Taste \x0a(KEY2)!' , font = font16, fill = COLORED)
        draw.text((1, 160), "FEHLER", font = font14, fill = COLORED)
        epd.display_partial_frame(image.rotate(90, expand=True), 0, 0, epd.height, epd.width, fast=True) 
        print("fehler")

if __name__ == '__main__':
    main()
    epd.sleep()
    
