# boids
This project simulates a flock of 'boids', or digital birds (or fish). Inspired by this [Boids lab](https://people.ece.cornell.edu/land/courses/ece4760/labs/s2021/Boids/Boids.html) from Cornell University.

![boids](https://user-images.githubusercontent.com/5415113/155014473-d556ddf3-7005-4e55-9ca3-7e104557f23b.gif)

The project was implemented in C for SDL, then translated to work with an Arduino-driven Adafruit 32x32 RGB LED Matrix. See code below for details on each implementation.

## C Project

### Dependencies
- `clang` compiler: https://clang.llvm.org/get_started.html
- `make`: http://www.gnu.org/software/make/
- Install SDL2: https://wiki.libsdl.org/Installation
  - Install dev version: `sudo apt-get install libsdl2-dev`

### Config
Configuration parameters can be found in [`boid.h`](https://github.com/bryanluu/boids/blob/main/src/c/boids.h).

### Run
- Run `make` file in Terminal at root: `make`
- Execute `bin/boids` after install

## Arduino + Adafruit Matrix
I used an [Arduino Metro M0 Express](https://www.adafruit.com/product/3505) paired with an [Adafruit 32x32 RGB LED matrix](https://www.adafruit.com/product/1484), see their respective sites for setup information.

The result of a simulation of 15 boids with some color dynamics:
![boids_matrix](https://user-images.githubusercontent.com/5415113/163721409-c5a7093c-6ee5-40fc-a2da-c90d15486f74.gif)
