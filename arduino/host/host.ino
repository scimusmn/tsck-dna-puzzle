#include "SerialController.h"
#include "Button.h"
#include "Rfid/Controller.h"
#include "Rfid/Tag.h"
#include "Queue.h"
#include "SoftwareSerial.h"

using namespace smm;

SerialController<> serial;
SoftwareSerial lights(9, 2);

#define NUM_RFID_READERS 6
#define MAX_RFID_TAGS 24
#define NUM_CATEGORIES 4
#define LED 13

RfidController<
    NUM_RFID_READERS,
    MAX_RFID_TAGS,
    NUM_CATEGORIES
    > rfid;

char genome[7] = "GGGGGG";
unsigned long rfidUpdateTime;

// rfid category handlers
int getReader(byte address) {
    return address - 0x70;
}

void sendGenome() {
    serial.send("genome", genome);
    lights.print("{genome:");
    lights.print(genome);
    lights.println("}");
}

void onReadG(byte addr, RfidTag& tag) {
    genome[getReader(addr)] = 'G';
    sendGenome();
}
void onReadC(byte addr, RfidTag& tag) {
    genome[getReader(addr)] = 'C';
    sendGenome();
}
void onReadT(byte addr, RfidTag& tag) {
    genome[getReader(addr)] = 'T';
    sendGenome();
}
void onReadA(byte addr, RfidTag& tag) {
    genome[getReader(addr)] = 'A';
    sendGenome();
}

RfidTag unknownTag;
bool teaching = false;

void onReadUnknown(byte address, RfidTag& tag) {
    serial.send("read-unknown", tag.toString().c_str());
    serial.send("read-unknown-on", address);
    for (int i=0; i<5; i++)
	unknownTag.tagData[i] = tag.tagData[i];
    teaching = true;
}


// buttons
Button teachGuanine(8);
Button teachCytosine(3);
Button teachThymine(5);
Button teachAdenine(4);
Button teachUndo(6);

void updateButtons() {
    teachGuanine.update();
    teachCytosine.update();
    teachThymine.update();
    teachAdenine.update();
    teachUndo.update();
}

void teach(byte category) {
    if (!teaching)
	return;
    serial.send("teaching-tag", unknownTag.toString().c_str());
    serial.send("teaching-as", category);
    rfid.teachTag(unknownTag, category);
    teaching = false;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {
    pinMode(LED, OUTPUT);

    serial.setup();
    lights.begin(115200);

    rfid.setup();
    rfid.addReader(0x70);
    rfid.addReader(0x71);
    rfid.addReader(0x72);
    rfid.addReader(0x73);
    rfid.addReader(0x74);
    rfid.addReader(0x75);

    rfid.onRead('G', onReadG);
    rfid.onRead('C', onReadC);
    rfid.onRead('T', onReadT);
    rfid.onRead('A', onReadA);
    rfid.onReadUnknown(onReadUnknown);
    rfid.onReadFailure([](byte addr) { serial.send("read-failure", addr); });

    rfidUpdateTime = millis() + 100;
    
    teachGuanine.onPress([]() { teach('G'); });
    teachCytosine.onPress([]() {teach('C'); });
    teachThymine.onPress([]() { teach('T'); });
    teachAdenine.onPress([]() { teach('A'); });
    teachUndo.onPress([]() { rfid.forgetLastTag(); });

    serial.send("arduino-ready");
    serial.send("known-tags", (int) rfid.numKnownTags());
    rfid.dumpTags();
    rfid.testReaders();
}


void loop() {
    if (teaching)
	digitalWrite(LED, 1);
    else
	digitalWrite(LED, 0);
    
    serial.update();
    if (millis() > rfidUpdateTime) {
	rfidUpdateTime = millis() + 100;
	rfid.update();
    }
	
    updateButtons();
}
