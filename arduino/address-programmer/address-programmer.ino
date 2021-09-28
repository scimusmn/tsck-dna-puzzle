#include <Wire.h>
#include "SerialController.h"

using namespace smm;

SerialController<> serial;

byte target = 0x70;

void setAddress(int addr) {
    Wire.beginTransmission(target);
    Wire.write(0x44);
    Wire.write(addr);
    Wire.endTransmission();

    getTag(addr);
}

void setTarget(int addr) {
    target = addr;
}

void getTarget() {
    serial.send("target", target);
}

void getTag(int addr) {
    Wire.requestFrom(addr, 6);
    serial.send("available-from", addr);
    serial.send("available", Wire.available());
    while(Wire.available())
	serial.send("data", Wire.read());
}

void scan() {
    for (int i=0; i<255; i++) {
	Wire.requestFrom(i, 6);
	if (Wire.available()) {
	    serial.send("found-chip", i);
	    while(Wire.available())
		Wire.read();
	}
    }
    serial.send("scan-done");
}

void setup() {
    Wire.begin();
    serial.setup();
    serial.addCallback("get-tag", getTag);
    serial.addCallback("set-address", setAddress);
    serial.addCallback("set-target", setTarget);
    serial.addCallback("get-target", getTarget);
    serial.addCallback("scan", scan);
    serial.send("arduino-ready");
    getTag(0x70);		       
}

void loop() {
    serial.update();
}
