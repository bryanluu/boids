#ifndef ADAPTER_H
#define ADAPTER_H

// Math
#define BOUND(l, x, h) ((x) > (h) ? (h) : ((x) < (l) ? (l) : (x))) // return x bounded between l and h

// Screen parameters
#define SCREEN_WIDTH 31
#define SCREEN_HEIGHT 31

int random(int max);

#endif
