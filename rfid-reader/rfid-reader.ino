#define MAX_RFID_READERS 6
#define MAX_TAG_CATEGORIES 4

#include "Rfid.h"

#define READER0 0x70
#define READER1 0x71
#define READER2 0x72
#define READER3 0x73
#define READER4 0x74
#define READER5 0x75

char genome[] = "AAAAAA";

struct Rfid *rfid;

enum {
  GUANINE,
  CYTOSINE,
  ADENINE,
  THYMINE,
};

unsigned long lastSendTime = 0;
#define SEND_TIMEOUT 10000

void sendGenome() {
  Serial.print("{genome:");
  Serial.print(genome);
  Serial.println("}");
}

void updateGenome(byte readerAddress, char base) {
  int index = readerAddress - 0x70;
  if (genome[index] != base) {
    genome[index] = base;
    sendGenome();
    lastSendTime = millis();
  }
}

void onGuanine(byte ra, byte id[5], void *d) {
  updateGenome(ra, 'G');
}

void onCytosine(byte ra, byte id[5], void *d) {
  updateGenome(ra, 'C');
}

void onAdenine(byte ra, byte id[5], void *d) {
  updateGenome(ra, 'A');
}

void onThymine(byte ra, byte id[5], void *d) {
  updateGenome(ra, 'T');
}

void onread_unknown(byte ra, byte id[5], void* d) {
  Serial.print("Read unknown tag [");
  Serial.print(idToString(id));
  Serial.print("] on reader ");
  Serial.println(ra, HEX);
}


void setup() {
  EEPROM.update(0, 0);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
  Serial.begin(115200);

  Serial.print("set up rfid...");
  digitalWrite(13, LOW);

  rfid = rfidSetup(4, 12, true);
  rfidAddReader(rfid, READER0);
  rfidAddReader(rfid, READER1);
  rfidAddReader(rfid, READER2);
  rfidAddReader(rfid, READER3);
  //rfidAddReader(rfid, READER4);
  rfidAddReader(rfid, READER5);

  rfidAddCategory(rfid, GUANINE, 2, onGuanine);
  rfidAddCategory(rfid, CYTOSINE, 3, onCytosine);
  rfidAddCategory(rfid, ADENINE, 4, onAdenine);
  rfidAddCategory(rfid, THYMINE, 5, onThymine);

  rfid->onunknown = onread_unknown;

  Serial.println(" done.");

  Serial.print("Known tags: ");
  Serial.println(rfid->numTags);

  for (int i = 0; i < rfid->numTags; i++) {
    struct Tag *tag = rfid->tags + i;
    Serial.print("[ ");
    Serial.print(idToString(tag->id));
    Serial.print(" ] - category ");
    Serial.println(tag->category);
  }
}


void loop() {
  rfidUpdate(rfid);
  if (millis() - lastSendTime > SEND_TIMEOUT) {
    sendGenome();
    lastSendTime = millis();
  }
}
