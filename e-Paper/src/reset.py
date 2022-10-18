
from util import *
def main():
    epd.init()
    image = Image.new('1', (epd.width, epd.height), 255)
    epd.display_frame(image)

if __name__ == '__main__':
    main()
    epd.sleep()