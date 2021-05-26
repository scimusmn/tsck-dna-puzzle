#ifndef SOYA_DISK_H
#define SOYA_DISK_H

class Ring {
private:
    CRGB *leds;
    int size;

    uint8_t angleToIndex(uint8_t angle) {
	uint8_t index = scale8(angle, size-1);
	if (index >= size)
	    index = size-1;
	return index;
    }
    
public:
        
    Ring(CRGB *leds, int size) :
	size(size), leds(leds)
    {}

    int getSize() { return size; }

    void set(uint8_t index, CRGB value) { leds[index] = value; }
    void setAll(CRGB value) {
	for (int i=0; i<size; i++)
	    leds[i] = value;
    }

    void setAngle(uint8_t angle, CRGB value) {
	leds[angleToIndex(angle)] = value;
    }
    void setAngleRange(uint8_t a0, uint8_t a1, CRGB value) {
	if (a1 < a0) {
	    setAngleRange(0, a1, value);
	    a1 = 255;
	}
	
	int startIndex = angleToIndex(a0);
	int stopIndex = angleToIndex(a1);

	for (int i=startIndex; i<=stopIndex; i++)
	    leds[i] = value;
    }
};


class Disk {
private:
    int numRings, numLeds;
    Ring *rings;
    CRGB *leds;

public:

    void setup(int n_rings, const int *sizes) {
	numRings = n_rings;

	numLeds = 0;
	for (int i=0; i<numRings; i++)
	    numLeds += sizes[i];

	leds = malloc(numLeds * sizeof(CRGB));
	rings = malloc(numRings * sizeof(Ring));

	int offset = 0;
	for (int i=0; i<numRings; i++) {
	    rings[i] = Ring(leds + offset, sizes[i]);
	    offset += sizes[i];
	}

	FastLED.addLeds<APA102, BGR>(leds, numLeds);
    }

    Ring& operator[](int index) {
	return rings[index];
    }
};

#endif
