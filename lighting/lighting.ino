#include <FastLED.h>

#define LED_DATA 4
#define LED_CLOCK 5

#define MAX_KEY_LEN 8
#define MAX_VAL_LEN 7

#include "SerialController.h"
#include "SoyaDisk.h"

Disk disk;

const int ringSizes[] = { 48, 44, 40, 32, 28, 24, 20, 12, 6, 1 };
char genome[] = { 'A', 'A', 'A', 'A', 'A', 'A', 0 };

CRGB colorA = CRGB(0xf5, 0x17, 0x20);
CRGB colorG = CRGB(0xfa, 0x26, 0xa0);
CRGB colorT = CRGB(0xf8, 0xd2, 0x10);
CRGB colorC = CRGB(0x2f, 0xf3, 0xe0);

SerialController serialController;

void processGenome(char *update) {
    int length = strlen(update);
    length = length < 6 ? length : 6;
    for (int i=0; i<length; i++)
	genome[i] = update[i];
    updateDiskFromGenome();
    serialController.sendMessage("update-genome", genome);
}

void setup() {
    Serial.begin(115200);
    serialController.setup(115200, false);
    serialController.sendMessage("setup", 1);
    serialController.addCallback("genome", processGenome);
    disk.setup(10, ringSizes);
    delay(100);
    processGenome("GATCGA");
}

CRGB baseColor(char base) {
    switch(base) {
    case 'A':
	return colorA;
    case 'G':
	return colorG;
    case 'T':
	return colorT;
    case 'C':
	return colorC;
    default:
	return CRGB::Black;
    }
}

void wedge(uint8_t a0, uint8_t a1, CRGB value) {
    for (int i=0; i<10; i++)
	disk[i].setAngleRange(a0, a1, value);
}

void updateDiskFromGenome() {
    const int smallWedge = 14;
    const int largeWedge = 84;
    int offset = -7;
    
    for (int i=0; i<6; i++) {
	//disk.leds[i] = baseColor(genome[i]);
	wedge(offset, offset+smallWedge, baseColor(genome[i]));
	wedge(offset + largeWedge, offset+largeWedge+smallWedge, baseColor(genome[i]));
	wedge(offset + 2*largeWedge, offset+(2*largeWedge)+smallWedge, baseColor(genome[i]));
	offset += smallWedge;
    }
    FastLED.show();
}

void loop() {
    serialController.update();
}
