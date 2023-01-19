# -*- coding: iso-8859-1 -*-

from util import *

# Leert den Bildschirm
def main():
    epd.init()
    image = Image.new('1', (epd.width, epd.height), 255)
    epd.display_frame(image)

if __name__ == '__main__':
    main()
    epd.sleep()
