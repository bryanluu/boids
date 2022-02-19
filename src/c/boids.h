#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "vector.h"

#define SCREEN_WIDTH 3000
#define SCREEN_HEIGHT 2000
#define SCREEN_DEPTH 255 // needs to be 255 for alpha
#define N_BOIDS 50
#define BOID_SIZE 5

struct image
{
  SDL_Surface* surface;
  SDL_Texture* tex;
  SDL_Rect rect;
  Vector3D position;
  Vector3D velocity;
};

typedef struct image Boid;

void placeBoid(Boid* boid);
int initBoid(SDL_Renderer* rend, Boid* boid);