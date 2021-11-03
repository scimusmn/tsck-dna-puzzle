#ifndef SOYA_DISK_H
#define SOYA_DISK_H

#ifndef LED_DATA
#define LED_DATA 4
#endif

#ifndef LED_CLOCK
#define LED_CLOCK 5
#endif

#define SOYA_NUM_LEDS 255

class Ring {
private:
    CRGB *m_leds;
    int size;

    uint8_t angleToIndex(uint8_t angle) {
	uint8_t index = scale8(angle, size-1);
	if (index >= size)
	    index = size-1;
	return index;
    }
    
public:
        
    void setup(CRGB *led_array, int ring_size) {
	m_leds = led_array;
	size = ring_size;
    }

    int getSize() { return size; }

    void set(uint8_t index, CRGB value) { m_leds[index] = value; }
    void setAll(CRGB value) {
	for (int i=0; i<size; i++)
	    m_leds[i] = value;
    }

    void setAngle(uint8_t angle, CRGB value) {
	m_leds[angleToIndex(angle)] = value;
    }
    void setAngleRange(uint8_t a0, uint8_t a1, CRGB value) {
	if (a1 < a0) {
	    setAngleRange(0, a1, value);
	    a1 = 255;
	}
	
	int startIndex = angleToIndex(a0);
	int stopIndex = angleToIndex(a1);

	for (int i=startIndex; i<=stopIndex; i++)
	    m_leds[i] = value;
    }
};


const int ringSizes[10] = { 48, 44, 40, 32, 28, 24, 20, 12, 6, 1 };

class Disk {
private:
    Ring rings[10];
    CRGB *m_leds;
    
public:
    void setup(CRGB *led_array) {
	m_leds = led_array;

	int offset = 0;
	for (int i=0; i<10; i++) {
	    rings[i].setup(m_leds + offset, ringSizes[i]);
	    offset += ringSizes[i];
	}
    }

    void setAll(CRGB color) {
	for (int i=0; i<SOYA_NUM_LEDS; i++)
	    m_leds[i] = color;
    }

    void wedge(uint8_t a0, uint8_t a1, CRGB value) {
	for (int i=0; i<10; i++)
	    rings[i].setAngleRange(a0, a1, value);
    }

    Ring& operator[](int index) {
	return rings[index];
    }
};

#endif
