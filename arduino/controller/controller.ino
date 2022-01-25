/* Copyright (c) 2022 Science Museum of Minnesota
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "SoftwareSerial.h"

#include "SerialController.h"
#include "Button.h"
#include "Rfid/Controller.h"
#include "Rfid/Tag.h"
#include "Queue.h"
#include "ProgrammerLeds.h"

using namespace smm;

// serial interfaces
SerialController<> serial;
SoftwareSerial lights(9, 2);


// the RFID controller
#define NUM_RFID_READERS 6
#define MAX_RFID_TAGS 48
#define NUM_CATEGORIES 4
#define LED 13

RfidController<
    NUM_RFID_READERS,
    MAX_RFID_TAGS,
    NUM_CATEGORIES
    > rfid;

#define LEDA 10
#define LEDT 9
#define LEDG 8
#define LEDC 7

ProgrammerLeds leds(LEDA, LEDT, LEDG, LEDC);

// genome
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

void handleKnownTag(byte addr, char base) {
   const int reader = getReader(addr);
   if (reader == 6) {
      // teaching reader
      leds.show(base);
   }
   else {
      // helix reader
      genome[reader] = base;
      sendGenome();
   }
}

void onReadG(byte addr, RfidTag& tag) {
   handleKnownTag(addr, 'G');
}
void onReadC(byte addr, RfidTag& tag) {
   handleKnownTag(addr, 'C');
}
void onReadT(byte addr, RfidTag& tag) {
   handleKnownTag(addr, 'T');
}
void onReadA(byte addr, RfidTag& tag) {
   handleKnownTag(addr, 'A');
}

RfidTag unknownTag;
bool teaching = false;

void onReadUnknown(byte address, RfidTag& tag) {
   serial.send("read-unknown", tag.toString().c_str());
   if (address != 0x76) {
      serial.send("read-unknown-on", address);
   }
   else {
      unknownTag = tag;
      teaching = true;
      leds.flash();
   }
}


// buttons
Button teachA(2);
Button teachT(3);
Button teachG(4);
Button teachC(5);
Button teachUndo(6);

void updateButtons() {
    teachG.update();
    teachC.update();
    teachT.update();
    teachA.update();
    teachUndo.update();
}

void teach(byte category) {
    if (!teaching)
	return;
    serial.send("teaching-tag", unknownTag.toString().c_str());
    serial.send("teaching-as", category);
    rfid.teachTag(unknownTag, category);
    teaching = false;
    leds.show(category);
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
    
    teachG.onPress([](void *d) { teach('G'); });
    teachC.onPress([](void *d) {teach('C'); });
    teachT.onPress([](void *d) { teach('T'); });
    teachA.onPress([](void *d) { teach('A'); });
    teachUndo.onPress([](void *d) { rfid.forgetLastTag(); });

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
