# Installation Waveshare e-Paper HAT

## Aktivierung der SPI Schnittstelle

Öffne das Terminal vom Pi und geb ein: \
`sudo raspi-config` \
Wähle 3 Interface Options > P4 SPI > JA > Ok > Finish \
`sudo reboot`

## Installation der Bibliotheken

### Installation der BCM2835 Bibliotheken

Öffne das Terminal vom Pi und geb ein: \
`wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz` \
`tar zxvf bcm2835-1.71.tar.gz` \
`cd bcm2835-1.71/` \
`sudo ./configure && sudo make && sudo make check && sudo make install`

### Installation der wiringPi Bibliotheken

Öffne das Terminal vom Pi und geb ein: \
`sudo apt-get install wiringpi` \
`wget https://project-downloads.drogon.net/wiringpi-latest.deb` \
`sudo dpkg -i wiringpi-latest.deb`

Installation überprüfen \
`gpio -v`\
Wenn _version 2.52_ erscheint war die Installation erfolgreich

### Installation der Python Bibliotheken

Öffne das Terminal vom Pi und geb ein: \
`sudo apt-get update` \
`sudo apt-get install python3-pip` \
`sudo apt-get install python3-pil` \
`sudo apt-get install python3-numpy` \
`sudo pip3 install RPi.GPIO` \
`sudo pip3 install spidev`

## Installation der Treiber

Öffne das Terminal vom Pi und geb ein: \
`cd ~/escape-game/e-Paper` \
`git clone https://github.com/elad661/rpi_epd2in7.git`
