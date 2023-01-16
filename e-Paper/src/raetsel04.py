# -*- coding: iso-8859-1 -*-

from util import *

import time

if sys.version_info.major < 3:
    input = raw_input

# Kommunikation mit Node-RED über stdin/stdout
def main():
    epd.init()  

    # IP Adresse und Linien
    anfangsImage = initRaetsel(initStatus("Aktiv", "4", getIP()), "4", "Rot sehen", COLORED)
    epd.display_frame(anfangsImage.rotate(90, expand=True))
    time.sleep(WARTEN)

    image=initStatus("Aktiv", "4", getIP())
    draw = ImageDraw.Draw(image)
    draw.text(((36), (epd.width/2)), "Farbwert: ",font = font18, fill = COLORED)
    epd.display_frame(image.rotate(90, expand=True))

    draw.text((((epd.height/2)), (epd.width/2)), "?",font = font18, fill = COLORED)
    epd.display_partial_frame(image.rotate(90, expand=True), 0, 0, epd.height, epd.width, fast=True)

    try:
        while True:
            text = input("> ") 
            if (text.__contains__("stop")):
                # Abbruch
                epd.sleep()
                print("Bye!")
                break
            elif (text.__contains__("korrekt")):
                # Wenn richtiger Farbwert gefunden wurde
                draw.text(((epd.height/2), (epd.width/2)), "?",font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2), (epd.width/2)), "falsch" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2), (epd.width/2)), "Korrekt!" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
                epd.sleep()
                print("Bye!")
                break
            elif(text.__contains__("fehler")):
                # Wenn Farbwert falsch ist
                draw.text((((epd.height/2)), (epd.width/2)), "?",font = font18, fill = UNCOLORED)
                draw.text((((epd.height/2)), (epd.width/2)), "falsch" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
                
    except KeyboardInterrupt:
        epd.sleep()
        print("Bye!")
        raise


if __name__ == '__main__':
    main()
    epd.sleep()
