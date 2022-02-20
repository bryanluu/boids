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

        // update flock
        for (int i = 0; i < N_BOIDS; i++)
        {
            updateBoid(&flock[i], flock);
            drawBoid(rend, &flock[i]);
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

/*
    Place the given Boid somewhere randomly along the screen
 */
void placeBoid(Boid* boid)
{
    boid->position.x = rand() % SCREEN_WIDTH;
    boid->position.y = rand() % SCREEN_HEIGHT;
    boid->position.z = rand() % SCREEN_DEPTH;
    int choice;
    choice = rand();
    boid->velocity.x = ((choice % 2) ? 1 : -1) * (0.5 * MIN_SPEED + (MAX_SPEED - MIN_SPEED) * ((choice % 1000) / 1000.0));
    choice = rand();
    boid->velocity.y = ((choice % 2) ? 1 : -1) * (0.5 * MIN_SPEED + (MAX_SPEED - MIN_SPEED) * ((choice % 1000) / 1000.0));
    choice = rand();
    boid->velocity.z = ((choice % 2) ? 1 : -1) * SEED_DEPTH_SPEED * ((choice % 1000) / 1000.0);
}

/*
    Initialize Boid parameters
 */
int initBoid(SDL_Renderer* rend, Boid* boid)
{
    int ret;

    // place boid somewhere
    placeBoid(boid);

    // create the surface
    boid->surface = SDL_CreateRGBSurfaceWithFormat(0, BOID_SIZE, BOID_SIZE, 32, SDL_PIXELFORMAT_RGBA32);
    ret = SDL_FillRect(boid->surface, NULL, SDL_MapRGBA(boid->surface->format, 255, 255, 255, boid->position.z));

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
    boid->rect.x = boid->position.x;
    boid->rect.y = boid->position.y;

    return ret;
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
    if (boid->position.z < MARGIN)
        boid->velocity.z += TURN_FACTOR;
    if (boid->position.z > SCREEN_HEIGHT - FRONT_MARGIN)
        boid->velocity.z -= TURN_FACTOR;
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
    if (boid->position.z < MARGIN)
        boid->position.z = MARGIN;
    if (boid->position.z > SCREEN_DEPTH)
        boid->position.z = SCREEN_DEPTH;
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

        Vector3D diff = sub(boid->position, other->position);
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
    Flutter depth-wise for nice effect
 */
void flutterDepth(Boid* boid)
{
    int choice = rand();
    boid->velocity.z += (choice % 2 ? 1 : -1) * FLUTTER_SPEED * (choice % DEPTH_FLUTTER);
}

/*
    Update loop for indivual Boid
 */
void updateBoid(Boid* boid, Boid* flock)
{
    flyWithFlock(boid, flock);
    avoidEdges(boid);
    constrainSpeed(boid);
    flutterDepth(boid);
    boid->position = add(boid->position, boid->velocity);
    constrainPosition(boid);
    boid->rect.x = boid->position.x;
    boid->rect.y = boid->position.y;
}

void drawBoid(SDL_Renderer* rend, Boid* boid)
{
    // create the surface
    boid->surface = SDL_CreateRGBSurfaceWithFormat(0, BOID_SIZE, BOID_SIZE, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(boid->surface, NULL, SDL_MapRGBA(boid->surface->format, 255, 255, 255, boid->position.z));
    SDL_DestroyTexture(boid->tex);
    boid->tex = SDL_CreateTextureFromSurface(rend, boid->surface);
    // clears main-memory
    SDL_FreeSurface(boid->surface);
}