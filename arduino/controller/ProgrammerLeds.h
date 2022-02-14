#ifndef PROGRAMMER_LEDS_H
#define PROGRAMMER_LEDS_H

#include "IntervalTimer.h"

class ProgrammerLeds {
 public:
   ProgrammerLeds(int ledA, int ledT, int ledG, int ledC) :
      ledA(ledA), ledT(ledT), ledG(ledG), ledC(ledC),
      flashing(false), ledFlash(400)
   {
      pinMode(ledA, OUTPUT);
      pinMode(ledT, OUTPUT);
      pinMode(ledG, OUTPUT);
      pinMode(ledC, OUTPUT);
   }

   void update() {
      if (flashing) {
	 ledFlash.update();
	 if (ledFlash.isOn()) {
	    digitalWrite(ledA, 1);
	    digitalWrite(ledT, 1);
	    digitalWrite(ledG, 1);
	    digitalWrite(ledC, 1);
	 }
	 else {
	    digitalWrite(ledA, 0);
	    digitalWrite(ledT, 0);
	    digitalWrite(ledG, 0);
	    digitalWrite(ledC, 0);
	 }
      }
   }

   void show(char base) {
      digitalWrite(ledA, 0);
      digitalWrite(ledT, 0);
      digitalWrite(ledG, 0);
      digitalWrite(ledC, 0);

      switch(base) {
      case 'A':
	 digitalWrite(ledA, 1);
	 break;

      case 'T':
	 digitalWrite(ledT, 1);
	 break;

      case 'G':
	 digitalWrite(ledG, 1);
	 break;

      case 'C':
	 digitalWrite(ledC, 1);
	 break;

      default:
	 // none should light up
	 break;
      }
   }

   void flash() {
      flashing = true;
   }

 private:
   bool flashing;
   IntervalTimer ledFlash;
   const int ledA, ledT, ledG, ledC;
};

#endif
