// Custom Boids demo on Adafruit's 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607
// 32x32 MATRICES DO NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Bryan Luu.
// BSD license, all text above must be included in any redistribution.

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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

// Boid parameters
#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

// Tunable parameters
#define N_BOIDS 15 // number of boids to simulate
#define BOID_SIZE 1 // how big are the boids?
#define MIN_SPEED 0.4 // min speed of a boid
#define MAX_SPEED 1 // max speed of a boid
#define MARGIN 4 // margins at which to start turning
#define TURN_FACTOR 0.15 // how quickly do boids avoid edges?
#define PROTECTED_RANGE 1.5 // the range at which boids avoid others
#define AVOID_FACTOR 0.05 // how quickly do boids avoid each other? 
#define VISIBLE_RANGE 5 // follow others within this range
#define MATCHING_FACTOR 0.06 // how quickly boids should follow flock?
#define CENTERING_FACTOR 0.005 // how closely do boids follow flock?

#define BOUND(l, x, h) ((x) > (h) ? (h) : ((x) < (l) ? (l) : (x))) // return x bounded between l and h

struct vector
{
  double x, y;
};

typedef struct vector Vector;

Vector add(Vector u, Vector v);
Vector sub(Vector u, Vector v);
Vector multiply(Vector u, double s);
double dot(Vector u, Vector v);
double length(Vector u);
void zero(Vector* u);

struct boid
{
  RGBmatrixPanel* surface;
  Vector position;
  Vector velocity;
  Vector closeness;
  Vector avgPosition;
  Vector avgVelocity;
  unsigned int neighbors;
};

typedef struct boid Boid;

void placeBoid(Boid* boid);
void constrainSpeed(Boid* boid);
void avoidEdges(Boid* boid);
void constrainPosition(Boid* boid);
void followNeighbors(Boid* boid);
void avoidOthers(Boid* boid);
void flyWithFlock(Boid* boid, Boid* flock);
void updateBoid(Boid* boid, Boid* flock);
void drawBoid(Boid* boid);

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

/////////// VECTOR CODE ////////

// add Vectors u & v together
Vector add(Vector u, Vector v)
{
  Vector result;
  result.x = u.x + v.x;
  result.y = u.y + v.y;
  return result;
}

// subtract v from u
Vector sub(Vector u, Vector v)
{
  Vector result;
  result.x = u.x - v.x;
  result.y = u.y - v.y;
  return result;
}

// calculate dot product of vectors
double dot(Vector u, Vector v)
{
  double result = (u.x * v.x + u.y * v.y);
  return result;
}

// calculate length of vector
double length(Vector u)
{
   return sqrt(dot(u, u));
}

// multiply by scalar
Vector multiply(Vector u, double s)
{
  Vector result;
  result.x = u.x * s;
  result.y = u.y * s;
  return result;
}

// zero the vector
void zero(Vector* u)
{
  u->x = 0;
  u->y = 0;
}

///////// BOID CODE /////////////

/*
    Place the given Boid somewhere randomly along the screen
 */
void placeBoid(Boid* boid)
{
    boid->position.x = random(SCREEN_WIDTH);
    boid->position.y = random(SCREEN_HEIGHT);
    long choice;
    choice = random(1000);
    boid->velocity.x = ((choice % 2) ? 1 : -1) * (0.5 * MIN_SPEED + (MAX_SPEED - MIN_SPEED) * (choice / 1000.0));
    choice = random();
    boid->velocity.y = ((choice % 2) ? 1 : -1) * (0.5 * MIN_SPEED + (MAX_SPEED - MIN_SPEED) * (choice / 1000.0));
}

/*
    Make Boid avoid screen edges
 */
void avoidEdges(Boid* boid)
{
    if (boid->position.x < MARGIN)
        boid->velocity.x += TURN_FACTOR;
    if (boid->position.x > SCREEN_WIDTH - MARGIN)
        boid->velocity.x -= TURN_FACTOR;
    if (boid->position.y < MARGIN)
        boid->velocity.y += TURN_FACTOR;
    if (boid->position.y > SCREEN_HEIGHT - MARGIN)
        boid->velocity.y -= TURN_FACTOR;
}

/*
    Ensure Boid speed remains within range
 */
void constrainSpeed(Boid* boid)
{
    double speed = sqrt(boid->velocity.x * boid->velocity.x + boid->velocity.y * boid->velocity.y);
    if (speed > MAX_SPEED)
    {
        boid->velocity.x = (boid->velocity.x * MAX_SPEED) / speed; 
        boid->velocity.y = (boid->velocity.y * MAX_SPEED) / speed; 
    }
    if (speed < MIN_SPEED)
    {
        boid->velocity.x = (boid->velocity.x * MIN_SPEED) / speed; 
        boid->velocity.y = (boid->velocity.y * MIN_SPEED) / speed; 
    }
}

/*
    Constrain Boid to always stay within screen
 */
void constrainPosition(Boid* boid)
{
    if (boid->position.x < 0)
        boid->position.x = 0;
    if (boid->position.x > SCREEN_WIDTH)
        boid->position.x = SCREEN_WIDTH;
    if (boid->position.y < 0)
        boid->position.y = 0;
    if (boid->position.y > SCREEN_HEIGHT)
        boid->position.y = SCREEN_HEIGHT;
}

/*
    Follow neighboring boids
 */
void followNeighbors(Boid* boid)
{
    if (boid->neighbors > 0)
    {
        boid->avgPosition = multiply(boid->avgPosition, 1.0 / boid->neighbors);
        boid->avgVelocity = multiply(boid->avgVelocity, 1.0 / boid->neighbors);
        boid->velocity = add(boid->velocity, multiply(sub(boid->avgVelocity, boid->velocity), MATCHING_FACTOR));
        boid->velocity = add(boid->velocity, multiply(sub(boid->avgPosition, boid->position), CENTERING_FACTOR));
    }
}

/*
    Avoid other boids
 */
void avoidOthers(Boid* boid)
{
    boid->velocity = add(boid->velocity, multiply(boid->closeness, AVOID_FACTOR));
}

/*
    Fly with the flock
 */
void flyWithFlock(Boid* boid, Boid* flock)
{
    zero(&boid->closeness);
    zero(&boid->avgPosition);
    zero(&boid->avgVelocity);
    boid->neighbors = 0;
    for (int i = 0; i < N_BOIDS; i++)
    {
        Boid* other = (flock + i);
        if (boid == other)
            continue;

        Vector diff = sub(boid->position, other->position);
        double dist = length(diff);
        if (dist < PROTECTED_RANGE)
            boid->closeness = add(boid->closeness, diff);
        if (dist < VISIBLE_RANGE)
        {
            boid->avgPosition = add(boid->avgPosition, other->position);
            boid->avgVelocity = add(boid->avgVelocity, other->velocity);
            boid->neighbors++;
        }
    }
    avoidOthers(boid);
    followNeighbors(boid);
}

/*
    Update loop for indivual Boid
 */
void updateBoid(Boid* boid, Boid* flock)
{
    flyWithFlock(boid, flock);
    avoidEdges(boid);
    constrainSpeed(boid);
    boid->position = add(boid->position, boid->velocity);
    constrainPosition(boid);
}

void drawBoid(Boid* boid)
{
    matrix.drawPixel(boid->position.x, boid->position.y, matrix.Color333(7, 7, 7));
}
