#include "test/minunit.h"
#include "test/suites.h"

typedef unsigned char uint8_t;

#define scale8(i, sc) ( (((unsigned int)i) * ((unsigned int)sc)) >> 8);

typedef struct mock_CRGB {
   int val;
} CRGB;

#include "sketch-lighting/SoyaDisk.h"


/* ~~~~~~~~ ring tests ~~~~~~~~ */

mu_test ring_4led();
mu_test ring_48led();


void soya_ring_tests() {
   mu_run_test("ring with 4 LEDs", ring_4led);
   mu_run_test("ring with 48 LEDs and angle wrapping", ring_4led);
}


/* ~~~~~~~~ disk tests ~~~~~~~~ */

mu_test disk_setall();
mu_test disk_wedge();

void soya_disk_tests() {
   mu_run_test("set all LEDs on a disk", disk_setall);
   mu_run_test("set all LEDs in a wedge", disk_wedge);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Ring tests
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

mu_test ring_4led() {
   Ring ring;
   CRGB leds[4];
   ring.setup(leds, 4);

   mu_assert_equal(ring.getSize(), 4);

   CRGB val1 = { 1 };

   ring.setAll(val1);
   mu_assert_equal(leds[0].val, val1.val);
   mu_assert_equal(leds[1].val, val1.val);
   mu_assert_equal(leds[2].val, val1.val);
   mu_assert_equal(leds[3].val, val1.val);

   CRGB val2 = { 2 };

   ring.set(1, val2);
   ring.set(3, val2);
   mu_assert_equal(leds[0].val, val1.val);
   mu_assert_equal(leds[1].val, val2.val);
   mu_assert_equal(leds[2].val, val1.val);
   mu_assert_equal(leds[3].val, val2.val);

   CRGB val3 = { 3 };
   ring.setAngleRange(0, 64, val3);
   mu_assert_equal(leds[0].val, val3.val);
   mu_assert_equal(leds[1].val, val2.val);
   mu_assert_equal(leds[2].val, val1.val);
   mu_assert_equal(leds[3].val, val2.val);

   ring.setAngleRange(128, 128+64, val3);
   mu_assert_equal(leds[0].val, val3.val);
   mu_assert_equal(leds[1].val, val2.val);
   mu_assert_equal(leds[2].val, val3.val);
   mu_assert_equal(leds[3].val, val2.val);

   ring.setAngleRange(64, 128, val1);
   mu_assert_equal(leds[0].val, val3.val);
   mu_assert_equal(leds[1].val, val1.val);
   mu_assert_equal(leds[2].val, val3.val);
   mu_assert_equal(leds[3].val, val2.val);

   ring.setAngleRange(128+64, 255, val1);
   mu_assert_equal(leds[0].val, val3.val);
   mu_assert_equal(leds[1].val, val1.val);
   mu_assert_equal(leds[2].val, val3.val);
   mu_assert_equal(leds[3].val, val1.val);

   return 0;
}


mu_test ring_48led() {
   Ring ring;
   CRGB leds[48];
   ring.setup(leds, 48);

   CRGB val0 = {0};
   ring.setAll(val0);
   for (int i=0; i<48; i++)
      mu_assert_equal(leds[i].val, 0);

   CRGB val1 = {1};
   ring.setAngleRange(0, 64, val1);
   for (int i=0; i<12; i++)
      mu_assert_equal(leds[i].val, 1);

   for (int i=12; i<48; i++)
      mu_assert_equal(leds[i].val, 0);

   ring.setAll(val0);
   ring.setAngleRange(192, 64, val1);
   for (int i=0; i<12; i++)
      mu_assert_equal(leds[i].val, 1);

   for (int i=12; i<36; i++)
      mu_assert_equal(leds[i].val, 0);

   for (int i=36; i<48; i++)
      mu_assert_equal(leds[i].val, 1);
   
   return 0;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Disk tests
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

mu_test disk_setall() {
   CRGB leds[Disk::ARRAY_SIZE];
   Disk disk;
   disk.setup(leds);

   CRGB val0 = {0};
   disk.setAll(val0);

   int offset = 0;
   
   for (int i=0; i<10; i++) {
      const int size = disk[i].getSize();
      for (int j=0; j<size; j++) {
	 mu_assert_equal(leds[offset + j].val, 0);
      }
      offset += size;
   }

   return 0;
}


mu_test disk_wedge() {
   CRGB leds[Disk::ARRAY_SIZE];
   Disk disk;
   disk.setup(leds);

   CRGB val0 = {0};
   CRGB val1 = {1};

   disk.setAll(val0);
   disk.wedge(0, 64, val1);

   int offset = 0;
   for (int i=0; 
}
