#include <SoftwareSerial.h>

#include <ArxContainer.h>

#include "Card.h"

SoftwareSerial rfid(2, 3);

Card card;

arx::map<String, String> cardMap;

void setup()
{
    cardMap["67 00 99 b9 6a"] = "guanine";
    cardMap["64 00 da 7e 67"] = "thymine";
    cardMap["64 00 dc 24 5c"] = "adenine";
    card.clear();
    
    Serial.begin(9600);
    while (!Serial) {}
    Serial.println("serial ready!");

    rfid.begin(9600);
}

void loop()
{
    while (rfid.available()) {
        char c = rfid.read();
        if (c > 31)
            card.addChar(c);
    }

    if (card.getStatus() == Card::Status::READ_OK) {
        String id = card.getId();
        String type = cardMap[id];
        Serial.println(type);
        card.clear();
    }
    else if (card.getStatus() == Card::Status::READ_BAD) {
        Serial.println("bad read!");
        card.clear();
    }
}
