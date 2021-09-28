#include <Wire.h>

#include "ID12LA.h"

#define PERIPHERAL_ADDRESS 0x7c
#define ANSWER_SIZE 5

Card card;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
}

void loop() {
    delay(500);

    Serial.println("Request latest card...");

    Wire.requestFrom(PERIPHERAL_ADDRESS, ANSWER_SIZE);

    for (int i=0; i<5; i++)
	card.id[i] = Wire.read();

    Serial.print("Peripheral response: ");
    Serial.println(card.getId());
}
