#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>


struct image
{
  SDL_Surface* surface;
  SDL_Texture* tex;
  SDL_Rect rect;
};

typedef struct image Boid;

int initBoid(SDL_Renderer* rend, Boid* boid);