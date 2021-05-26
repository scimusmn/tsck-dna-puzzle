#include <FastLED.h>

#include "SoyaDisk.h"

Disk disk;

const int ringSizes[] = { 48, 44, 40, 32, 28, 24, 20, 12, 6, 1 };
char genome[] = { 'A', 'A', 'A', 'A', 'A', 'A' };

void setup() {
    Serial.begin(9600);
    disk.setup(10, ringSizes);
}

/*void updateDisk(uint8_t rotation) {
    int pos = NUM_LEDS-1;
    for (int disk = 0; disk < 10; disk++) {
	for (int i = 0; i<diskSize[disk]; i++) {
	    uint8_t hue = int(255 * (float(i)/diskSize[disk])) + rotation;
	    leds[pos] = CHSV(hue, 255, 40);
	    pos--;
	}
    }

    FastLED.show();
    }*/


const int wedgeAngle = 1;

void loop() {
    for (int angle=0; angle<256; angle++) {
	for (int diskIndex=0; diskIndex<10; diskIndex++)
	    disk[diskIndex].setAngleRange(angle, angle+wedgeAngle, CRGB::Red);
	FastLED.show();
	delay(10);
	for (int diskIndex=0; diskIndex<10; diskIndex++)
	    disk[diskIndex].setAngleRange(angle, angle+wedgeAngle, CRGB::Black);
    }
}
