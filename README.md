# RGB Kpop Lightstick

## Installation Guide

This Guide from Adafruits shows how to install the Arduino IDE that supports Adafruits

<https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide>

### Latest Arduino IDE

<http://www.arduino.cc/en/Main/Software>

### Adafruits Trinket USB Driver

<https://github.com/adafruit/Adafruit_Windows_Drivers/releases/latest>

### Adafruits Board Manager

If the Board is not getting registered in the Arduino IDE set the Boardmanager in Preferences:
<https://adafruit.github.io/arduino-board-index/package_adafruit_index.json>

-   Install the Adafruits AVR Boards in the Board Manager

### Uploading Code

-   Select the Tools->Programmer to be `USBtinyISP`
-   Select the Tools->Board to be `Adafruit Trinket (ATtiny85 @ 8MHz)` or similar from Adafruits
-   To finally upload code to the trinket select "Upload using programmer" instead of the regular upload

## Circuit Image

![Circuit Image](rgbcircuitc600.png)
