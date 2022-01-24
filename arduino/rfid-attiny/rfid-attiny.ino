#include <Wire.h>
#include "ID12LA.h"

#define PERIPHERAL_ADDRESS 0x75
#define ANSWER_SIZE 5
#define LED_PIN 13

ID12LA reader;
Card card;

void onread(Card& c, void* data)
{
    card = c;
    digitalWrite(LED_PIN, HIGH);
    delay(10);
    digitalWrite(LED_PIN, LOW);
}

void setup() {
    reader.setup(2, onread, NULL);
    memset(card.id, 0x0a, 5);

    pinMode(LED_PIN, OUTPUT);
    
    Wire.begin(PERIPHERAL_ADDRESS);
    Wire.onRequest(requestEvent);
    //Wire.onReceive(receiveEvent);
}

void loop()
{
    reader.update();
}

void requestEvent()
{
    Wire.write(card.id, sizeof(card.id));
}
	    
