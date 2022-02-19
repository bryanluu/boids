#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

int main(int argc, char *argv[])
{

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    1000, 1000, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    // creates a surface
    SDL_Surface* surface;
    surface = SDL_CreateRGBSurfaceWithFormat(0, 100, 100, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 0, 0, 255));


    SDL_Surface* surface2;
    surface2 = SDL_CreateRGBSurfaceWithFormat(0, 100, 100, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface2, NULL, SDL_MapRGBA(surface2->format, 255, 0, 0, 255));

    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture* tex2 = SDL_CreateTextureFromSurface(rend, surface2);

    // clears main-memory
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);

    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;
    SDL_Rect dest2;

    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(tex, NULL, NULL, &dest2.w, &dest2.h);

    // adjust height and width of our image box.
    dest.w /= 6;
    dest.h /= 6;
    dest2.w /= 6;
    dest2.h /= 6;

    // sets initial x-position of object
    dest.x = (1000 - dest.w) / 2;

    // sets initial y-position of object
    dest.y = (1000 - dest.h) / 2;

    // controls animation loop
    int close = 0;

    // speed of box
    int speed = 300;

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
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= speed / 30;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= speed / 30;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += speed / 30;
                    break;
                default:
                    break;
                }
            }
        }

        // right boundary
        if (dest.x + dest.w > 1000)
            dest.x = 1000 - dest.w;

        // left boundary
        if (dest.x < 0)
            dest.x = 0;

        // bottom boundary
        if (dest.y + dest.h > 1000)
            dest.y = 1000 - dest.h;

        // upper boundary
        if (dest.y < 0)
            dest.y = 0;

        // clears the screen
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderCopy(rend, tex2, NULL, &dest2);

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }

    // destroy texture
    SDL_DestroyTexture(tex);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);
    
    // close SDL
    SDL_Quit();

    return 0;
}
