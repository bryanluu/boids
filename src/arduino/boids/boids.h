#ifndef BOIDS_H
#define BOIDS_H

#include "adapter.h"
#include "vector.h"

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
#define LONELY_LIMIT 2 // below how many separated boids is considered 'lonely'?

struct boid
{
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

#endif
