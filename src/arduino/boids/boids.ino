// Custom Boids demo on Adafruit's 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607
// 32x32 MATRICES DO NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Bryan Luu.
// BSD license, all text above must be included in any redistribution.

#include "vector.h"
#include "boids.h"
#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// Boid drawing
#define DEFAULT_COLOR (matrix.Color333(4, 4, 7)) // color of a boid by default
#define DANGER_COLOR (matrix.Color333(7, 2, 2)) // color of a boid in danger
#define LONELY_COLOR (matrix.Color333(7, 7, 1)) // color of a boid that is lonely
#define SLOW_COLOR (matrix.Color333(1, 1, 7)) // color of a slow boid


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

void drawBoid(Boid* boid)
{
    byte x = BOUND(0, boid->position.x, SCREEN_WIDTH);
    byte y = BOUND(0, boid->position.y, SCREEN_HEIGHT);
    uint16_t color = DEFAULT_COLOR;

    if ((x == 0) || (x == SCREEN_WIDTH) || (y == 0) || (y == SCREEN_HEIGHT)) // boid is too close to the wall
    {
      color = DANGER_COLOR;
    }
    else if (boid->neighbors < LONELY_LIMIT) // boid is not part of a flock
    {
      color = LONELY_COLOR;
    }
    else
    {
      double frac_speed_limit = abs(MAX_SPEED - length(boid->velocity))/MAX_SPEED;
      if (frac_speed_limit > 0.25) // boid is too slow
        color = SLOW_COLOR;
    }
    
    matrix.drawPixel(x, y, color);
}

// creates flock
Boid flock[N_BOIDS];

void setup() {
  randomSeed(analogRead(0));
  matrix.begin();

  // position boids
  for (int i = 0; i < N_BOIDS; i++)
  {
      placeBoid(&flock[i]);
  }
}

void loop() {
  // Clear background
  matrix.fillScreen(0);

  // update flock
  for (int i = 0; i < N_BOIDS; i++)
  {
      updateBoid(&flock[i], flock);
      drawBoid(&flock[i]);
  }

#if !defined(__AVR__)
  // On non-AVR boards, delay slightly so screen updates aren't too quick.
  delay(20);
#endif

  // Update display
  matrix.swapBuffers(false);
}

