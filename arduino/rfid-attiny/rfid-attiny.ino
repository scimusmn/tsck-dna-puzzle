#include "ID12LA.h"
#include "Tag.h"

#include <Wire.h>

using namespace smm;

RfidTag tag;
ID12LA reader;


#define ADDRESS 0x76

#define CMD_SET_ADDR 0x44
#define CMD_CLEAR_TAG 0x54

#define READ_PIN 0
#define LED_PIN 10
#define FLASH_TIME 20


void flash() {
    digitalWrite(LED_PIN, 1);
    delay(FLASH_TIME);
    digitalWrite(LED_PIN, 0);
}


void onReadTag(RfidTag t) {
    memcpy(tag.tagData, t.tagData, 5*sizeof(byte));
    flash();
}


void sendTag() {
    Wire.write(tag.tagData, 5);
    Wire.write(tag.checksum());
}


void clearTag() {
    memset(tag.tagData, 0xff, 5 * sizeof(byte));
}


void processCommand(int n) {
    if (n < 1)
	return;

    byte command = Wire.read();
    if (command == CMD_CLEAR_TAG)
	clearTag();
}


void setup() {
    reader.setup(READ_PIN, onReadTag);
    
    Wire.begin(ADDRESS);
    Wire.onRequest(sendTag);
    Wire.onReceive(processCommand);

    flash();
}

void loop() {
    reader.update();
}
