#ifndef PROGRAMMER_LEDS_H
#define PROGRAMMER_LEDS_H

#include "IntervalTimer.h"
#include "TimeoutTimer.h"


/* The LEDs on the programming board indicate the state of the
 * chip that was just scanned. If it is already known, then
 * the LED corresponding to what it is should light up; otherwise,
 * every LED should flash until the user presses one of the teach buttons
 * to set a value for it.
 */

class LedTimeout : public TimeoutTimer {
public:
	int ledA, ledT, ledG, ledC;
	LedTimeout(int ledA, int ledT, int ledG, int ledC, int timeout) :
		ledA(ledA), ledT(ledT), ledG(ledG), ledC(ledC),
		TimeoutTimer(timeout)
	{}
	
	void onDeactivate() {
		// turn off all LEDs
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

	// set every LED to the same state
	void setAll(int state) {
		digitalWrite(ledA, state);
		digitalWrite(ledT, state);
		digitalWrite(ledG, state);
		digitalWrite(ledC, state);
	}


	// update the LEDs
	void update() {
		ledTimeout.update();
	
		if (flashing) {
			ledFlash.update();
			if (ledFlash.isOn()) {
				setAll(1);
			}
			else {
				setAll(0);
			}
		}
	}

	// light up the LED for a specific base
	void show(char base) {
		flashing = false;
		setAll(0);

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
