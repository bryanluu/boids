#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "vector.h"

#define SCREEN_WIDTH 3000
#define SCREEN_HEIGHT 2000
#define SCREEN_DEPTH 255 // needs to be 255 for alpha

// Tunable parameters
#define N_BOIDS 500 // number of boids to simulate
#define BOID_SIZE 5 // how big are the boids?
#define MIN_SPEED 10 // min speed of a boid
#define MAX_SPEED 15 // max speed of a boid
#define SEED_DEPTH_SPEED 15 // random seed for depth boid speed
#define MARGIN 100 // margins at which to start turning
#define FRONT_MARGIN 50 // margin at the front of screen
#define TURN_FACTOR 2 // how quickly do boids avoid edges?
#define PROTECTED_RANGE 20 // the range at which boids avoid others
#define AVOID_FACTOR 0.2 // how quickly do boids avoid each other? 
#define VISIBLE_RANGE 150 // follow others within this range
#define MATCHING_FACTOR 0.1 // how quickly boids should follow flock?
#define CENTERING_FACTOR 0.005 // how closely do boids follow flock?
#define DEPTH_FLUTTER 50 // how much do boids flutter between depths?
#define FLUTTER_SPEED 0.5 // how quickly do boids flutter?

struct boid
{
  SDL_Surface* surface;
  SDL_Texture* tex;
  SDL_Rect rect;
  Vector3D position;
  Vector3D velocity;
  Vector3D closeness;
  Vector3D avgPosition;
  Vector3D avgVelocity;
  unsigned int neighbors;
};

typedef struct boid Boid;

void placeBoid(Boid* boid);
int initBoid(SDL_Renderer* rend, Boid* boid);
void constrainSpeed(Boid* boid);
void avoidEdges(Boid* boid);
void constrainPosition(Boid* boid);
void followNeighbors(Boid* boid);
void avoidOthers(Boid* boid);
void flyWithFlock(Boid* boid, Boid* flock);
void flutterDepth(Boid* boid);
void updateBoid(Boid* boid, Boid* flock);
void drawBoid(SDL_Renderer* rend, Boid* boid);