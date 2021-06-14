#include "ID12LA.h"

ID12LA reader;

void onread(Card& card, void* data)
{
    Serial.println(card.getId());
}


void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    reader.setup(2, onread, NULL);
}

void loop() { reader.update(); }
