#include "boids.h"
#include <cmath>

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
    boid->position.x = BOUND(0, boid->position.x, SCREEN_WIDTH);
    boid->position.y = BOUND(0, boid->position.y, SCREEN_HEIGHT);
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

