#include "Arduino.h"

// ensure this library description is only included once
#ifndef TimeoutTimer_h
#define TimeoutTimer_h

typedef unsigned long millis_t;

class TimeoutTimer {
private:
   millis_t m_onTime, m_lastTime;
   bool m_isOn = false;
public:
   TimeoutTimer (millis_t onTime, bool initialState=false) :
      m_onTime(onTime), m_isOn(initialState)
   {
      m_lastTime = millis();
   }

   void update() {
      if (m_isOn) {
	 if (millis() - m_lastTime > m_onTime) {
	    // on too long!
	    m_isOn = false;
	    onDeactivate();
	 }
      }
   }

   virtual void onDeactivate() {
      Serial.println("bad!");
   }

   void trigger() {
      m_isOn = true;
      m_lastTime = millis();
   }

   bool isOn() {
      return m_isOn;
   }
};

#endif
