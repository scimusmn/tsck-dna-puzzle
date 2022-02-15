rfid-attiny
===========

This program is intended for ATTiny84 chips monitoring ID-2LA, ID-12LA, or ID-20LA 125kHz RFID reader modules.


setup
-----

Tested using the [ATTinyCore] board pack.

Obtain a fresh ATTiny84. Open the sketch in the Arduino IDE, and select the following under the "Tools" menu:

* Board: ATtiny24/44/84(a) (No bootloader)
* Chip: ATtiny84a
* Clock Source: 8 MHz (internal)
* Pin Mapping: clockwise
* Programmer: USBTinyISP (ATTinyCore) SLOW

Then click `Tools > Burn Bootloader` (I know, I know, it says "no bootloader", but this is needed to set the clock speed).

Set the `ADDRESS` macro to whatever you'd like the device address to be, and then click `Sketch > Upload Using Programmer`. Done!
