#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "vector.h"

#define SCREEN_WIDTH 3000
#define SCREEN_HEIGHT 2000
#define SCREEN_DEPTH 255 // needs to be 255 for alpha

// Tunable parameters
#define N_BOIDS 100 // number of boids to simulate
#define BOID_SIZE 5 // how big are the boids?
#define MIN_SPEED 10 // min speed of a boid
#define MAX_SPEED 15 // max speed of a boid
#define SEED_DEPTH_SPEED 5 // random seed for depth boid speed
#define MARGIN 100 // margins at which to start turning
#define FRONT_MARGIN 50 // margin at the front of screen
#define TURN_FACTOR 2 // how quickly do boids avoid edges?
#define PROTECTED_RANGE 50 // the range at which boids avoid others
#define AVOID_FACTOR 1 // how quickly do boids avoid each other? 

struct boid
{
  SDL_Surface* surface;
  SDL_Texture* tex;
  SDL_Rect rect;
  Vector3D position;
  Vector3D velocity;
  Vector3D closeness;
};

typedef struct boid Boid;

void placeBoid(Boid* boid);
int initBoid(SDL_Renderer* rend, Boid* boid);
void constrainSpeed(Boid* boid);
void avoidEdges(Boid* boid);
void constrainPosition(Boid* boid);
void avoidOthers(Boid* boid, Boid* flock);
void updateBoid(Boid* boid, Boid* flock);