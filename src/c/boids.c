#include "boids.h"

int main(int argc, char *argv[])
{

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("Boids", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    // creates flock
    Boid flock[N_BOIDS];
    for (int i = 0; i < N_BOIDS; i++)
    {
        initBoid(rend, &flock[i]);
    }

    // controls animation loop
    int close = 0;

    // animation loop
    while (!close) {
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;

            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    close = 1;
                    break;
                default:
                    break;
                }
            }
        }

        // clears the screen
        SDL_RenderClear(rend);
        for (int i = 0; i < N_BOIDS; i++)
        {
            SDL_RenderCopy(rend, flock[i].tex, NULL, &flock[i].rect);
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }

    // destroy texture
    for (int i = 0; i < N_BOIDS; i++)
    {
        SDL_DestroyTexture(flock[i].tex);
    }

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);
    
    // close SDL
    SDL_Quit();

    return 0;
}

int initBoid(SDL_Renderer* rend, Boid* boid)
{
    int ret;
    int x, y, z;

    x = rand() % SCREEN_WIDTH;
    y = rand() % SCREEN_HEIGHT;
    z = rand() % SCREEN_DEPTH;

    // create the surface
    boid->surface = SDL_CreateRGBSurfaceWithFormat(0, BOID_SIZE, BOID_SIZE, 32, SDL_PIXELFORMAT_RGBA32);
    ret = SDL_FillRect(boid->surface, NULL, SDL_MapRGBA(boid->surface->format, 255, 255, 255, z));

    if (ret)
    {
        return ret;
    }

    // loads image to our graphics hardware memory.
    boid->tex = SDL_CreateTextureFromSurface(rend, boid->surface);

    // clears main-memory
    SDL_FreeSurface(boid->surface);

    // connects our texture with rect to control position
    ret = SDL_QueryTexture(boid->tex, NULL, NULL, &(boid->rect.w), &(boid->rect.h));

    // position Boid
    boid->rect.x = x;
    boid->rect.y = y;

    return ret;
}