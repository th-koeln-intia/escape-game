# -*- coding: iso-8859-1 -*-

from util import *

# Zeigt Titelbild an
def main():
        epd.init()
        epd.display_frame(initRaetsel(initStatus("Aktiv", "2", getIP()), '2', 'Es werde Licht', COLORED).rotate(90, expand=True))
        epd.sleep()  

if __name__ == '__main__':
    main()
    epd.sleep()       
