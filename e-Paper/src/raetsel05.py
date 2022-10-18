from util import *

def main():
        epd.init()
        epd.display_frame(initRaetsel(initStatus("Aktiv", "5", getIP()), '5', 'Keine Bewegung!', COLORED).rotate(90, expand=True))
        epd.sleep()  

if __name__ == '__main__':
    main()
    epd.sleep()      