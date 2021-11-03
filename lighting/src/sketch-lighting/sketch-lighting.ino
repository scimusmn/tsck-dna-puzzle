#include <FastLED.h>

#define LED_DATA 4
#define LED_CLOCK 5

#define MAX_KEY_LEN 8
#define MAX_VAL_LEN 7

#include "SerialController.h"
SerialController serialController;

#include "SoyaDisk.h"

Disk diskA, diskB;
CRGB leds[SOYA_NUM_LEDS];

char genome[] = { 'A', 'A', 'A', 'A', 'A', 'A', 0 };

/*
CRGB colorA = CRGB(0xff, 0x00, 0x00); // #ff0000 (red)
CRGB colorT = CRGB(0x22, 0xca, 0xb1); // #22cab1 (cyan)
CRGB colorG = CRGB(0xa4, 0x00, 0xb1); // #a400b1 (purple)
CRGB colorC = CRGB(0xf4, 0xdc, 0x00); // #f4dc00 (yellow)
*/


CRGB colorA = CRGB(0x4a, 0x22, 0x1d); // #ff0000 (brown)
CRGB colorT = CRGB(0x2f, 0x82, 0x37); // #22cab1 (green)
CRGB colorG = CRGB(0x1c, 0x6d, 0x9c); // #a400b1 (blue)
CRGB colorC = CRGB(0xcb, 0xa0, 0x29); // #f4dc00 (yellow)


void printId(byte id[5]) {
    Serial.print("[ ");
    for(int i=0; i<6; i++) {
	Serial.print(id[i], HEX); Serial.print(" ");
    }
    Serial.println("]");
}

void processGenome(char *update) {
    int length = strlen(update);
    length = length < 6 ? length : 6;
    for (int i=0; i<length; i++)
	genome[i] = update[i];
    updateDiskFromGenome(diskA);
    updateDiskFromGenome(diskB);
    FastLED.show();
    serialController.sendMessage("update-genome", genome);
}

void setup() {
    serialController.setup(115200, false);
    serialController.sendMessage("begin-setup");
    
    serialController.sendMessage("allocate-memory");
    diskA.setup(leds);
    serialController.sendMessage("setup leds");
    FastLED.addLeds<APA102, LED_DATA, LED_CLOCK, BGR>(leds, SOYA_NUM_LEDS);
    FastLED.addLeds<APA102, LED_DATA, LED_CLOCK, BGR>(leds, SOYA_NUM_LEDS);

    FastLED.setBrightness(16);
    
    delay(100);
    serialController.sendMessage("add LEDs");
    processGenome("GATCGA");
    //diskA.setAll(CRGB::Red);
    FastLED.show();

    
    
    serialController.sendMessage("finish-setup", 1);
    serialController.addCallback("genome", processGenome);
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

void updateDiskFromGenome(Disk& disk) {
    const int smallWedge = 21;
    const int largeWedge = 128;
    int offset = 0;
        
    for (int i=0; i<6; i++) {
	disk.wedge(offset, offset+smallWedge, baseColor(genome[i]));
	disk.wedge(offset + largeWedge, offset+largeWedge+smallWedge, baseColor(genome[i]));
	disk.wedge(offset + 2*largeWedge, offset+(2*largeWedge)+smallWedge, baseColor(genome[i]));
	offset += smallWedge;
    }

    for (int i=6; i<10; i++)
	disk[i].setAll(CRGB::Black);
}

void loop() {
    serialController.update();
}
