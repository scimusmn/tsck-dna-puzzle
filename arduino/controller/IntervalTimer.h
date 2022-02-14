#include "Arduino.h"

// ensure this library description is only included once
#ifndef IntervalTimer_h
#define IntervalTimer_h

typedef unsigned long millis_t;

class IntervalTimer {
private:
   millis_t m_onTime, m_offTime, m_lastTime;
   bool m_isOn = false;
public:
   IntervalTimer(millis_t onTime, millis_t offTime=0, bool initialState=false) :
      m_onTime(onTime), m_offTime(offTime), m_lastTime(0),
      m_isOn(initialState)
   {
      if (m_offTime == 0)
	 m_offTime = m_onTime;
   }

   void update() {
      millis_t now = millis();
      if (m_isOn) {
	 if (now - m_lastTime > m_onTime) {
	    // on too long!
	    m_isOn = false;
	    m_lastTime = now;
	 }
      }
      else {
	 if (now - m_lastTime > m_offTime) {
	    // off too long!
	    m_isOn = true;
	    m_lastTime = now;
	 }
      }
   }

   bool isOn() {
      return m_isOn;
   }
};

#endif
