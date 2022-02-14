#ifndef PROGRAMMER_LEDS_H
#define PROGRAMMER_LEDS_H

#include "IntervalTimer.h"
#include "TimeoutTimer.h"

class LedTimeout : public TimeoutTimer {
public:
   int ledA, ledT, ledG, ledC;
   LedTimeout(int ledA, int ledT, int ledG, int ledC, int timeout) :
      ledA(ledA), ledT(ledT), ledG(ledG), ledC(ledC),
      TimeoutTimer(timeout)
   {}
   
   void onDeactivate() {
      digitalWrite(ledA, 0);
      digitalWrite(ledT, 0);
      digitalWrite(ledG, 0);
      digitalWrite(ledC, 0);
   }
};



class ProgrammerLeds {
 public:
   ProgrammerLeds(int ledA, int ledT, int ledG, int ledC) :
      ledA(ledA), ledT(ledT), ledG(ledG), ledC(ledC),
      flashing(false), ledFlash(400),
      ledTimeout(ledA, ledT, ledG, ledC, 1000)
   {
      pinMode(ledA, OUTPUT);
      pinMode(ledT, OUTPUT);
      pinMode(ledG, OUTPUT);
      pinMode(ledC, OUTPUT);
   }

   void update() {
      ledTimeout.update();
      
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
      flashing = false;
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

      ledTimeout.trigger();
   }

   void flash() {
      flashing = true;
   }

 private:
   bool flashing;
   IntervalTimer ledFlash;
   LedTimeout ledTimeout;
   const int ledA, ledT, ledG, ledC;
};

#endif
