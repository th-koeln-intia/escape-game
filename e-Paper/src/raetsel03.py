# -*- coding: iso-8859-1 -*-
from util import *

if sys.version_info.major < 3:
    input = raw_input

# leere kaestchen
def resetMorse(image):
    draw = ImageDraw.Draw(image)
    draw.rectangle((9, ((epd.width)/2)+14, 38, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((45, ((epd.width)/2)+14, 74, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((81, ((epd.width)/2)+14, 110, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((117, ((epd.width)/2)+14, 146, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((153, ((epd.width)/2)+14, 182, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((189, ((epd.width)/2)+14, 218, ((epd.width)/2)-15), outline = COLORED)
    draw.rectangle((225, ((epd.width)/2)+14,254, ((epd.width)/2)-15), outline = COLORED)
    return image

def main():

    print("initializing...")
    epd.init()
    epd.display_frame(initRaetsel(initStatus("Aktiv", "3", getIP()),"3", "Sesam \xf6ffne dich!", COLORED).rotate(90, expand=True))
    time.sleep(WARTEN)

    try:
        img= Image.open(os.path.join(picdir, 'leeresMorse.bmp'))
    except:
        img=resetMorse(Image.new('1', (epd.height, epd.width), 255))
        img.save(os.path.join(picdir, 'leeresMorse.bmp'))
        
    image=initStatus("Aktiv","3", getIP(),img)
    draw= ImageDraw.Draw(image)

    epd.display_frame(image.rotate(90, expand=True))
    
    print("Ready.")
    try:
        while True:
            text = input("> ") 
            if (text.__contains__("stop")):
                epd.sleep()
                print("Bye!")
                break
            elif(text.__contains__("1")):
                draw.text(((epd.height/2)-24, (epd.width/2)+36), "Fehler" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = COLORED)
                draw.rectangle((19,((epd.width)/2)+1,28,((epd.width)/2)-1), fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)

            elif(text.__contains__("2")):
                draw.rectangle((59,((epd.width)/2)+1,62,((epd.width)/2)-1), fill = COLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)

            elif(text.__contains__("3")):
                draw.rectangle((91,((epd.width)/2)+1,100,((epd.width)/2)-1), fill = COLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
            elif(text.__contains__("4")):
                draw.rectangle((131,((epd.width)/2)+1,134,((epd.width)/2)-1), fill = COLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
            elif(text.__contains__("5")):
                draw.rectangle((170,((epd.width)/2)+1,167,((epd.width)/2)-1), fill = COLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
            elif(text.__contains__("6")):
                draw.rectangle((199,((epd.width)/2)+1,208,((epd.width)/2)-1), fill = COLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
            elif(text.__contains__("7")):
                draw.rectangle((239,((epd.width)/2)+1,242,((epd.width)/2)-1), fill=0)  
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Lang" , font = font18, fill = UNCOLORED)
                draw.text(((epd.height/2)-16, (epd.width/2)+36), "Kurz" , font = font18, fill = COLORED)
                draw.text(((epd.height/2)-64, (epd.width/2)-56), "Code korrekt" , font = font22, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
                break
            
            elif(text.__contains__("fehler")):
                image=(initStatus("Aktiv", "3", getIP(),Image.open(os.path.join(picdir, 'leeresMorse.bmp'))))
                draw= ImageDraw.Draw(image)

                img=image.rotate(90, expand=True)
                epd.display_frame(img)

                draw.text(((epd.height/2)-24, (epd.width/2)+36), "Fehler" , font = font18, fill = COLORED)
                img=image.rotate(90, expand=True)
                epd.display_partial_frame(img, 0, 0, epd.height, epd.width, fast=True)
                
    except KeyboardInterrupt:
        epd.sleep()
        print("Bye!")
        raise

if __name__ == '__main__':
    main()
    epd.sleep()

