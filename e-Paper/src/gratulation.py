from util import *

def main():
        epd.init()
        image=initStatus("Aktiv", "5", getIP())
        draw=ImageDraw.Draw(image)
        draw.text((20, epd.width/2-10),'Herzlichen Gl\xfcckwunsch!', font = font18, fill = COLORED)
        epd.display_frame(image.rotate(90, expand=True))
        epd.sleep()  

if __name__ == '__main__':
    main()
    epd.sleep()   