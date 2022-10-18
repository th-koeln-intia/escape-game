
from util import *

def main():
    epd.init()
    image=initStatus("Aktiv", "5", getIP()+":8082").convert("RGBA")
    image2 = Image.open(os.path.join(picdir, 'qrcode.png')).convert("RGBA")
    
    image.paste(image2,(63,22),image2)
    epd.display_frame(image.convert("1").rotate(90, expand=True))
    epd.sleep()  

if __name__ == '__main__':
    main()
    epd.sleep()       