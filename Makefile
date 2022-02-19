# A simple Makefile for compiling small SDL projects

S_DIR=src/c
B_DIR=bin

# set the compiler
CC := clang

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm

# add header files here
HDRS := $(S_DIR)/boids.h $(S_DIR)/vector.h

# add source files here
SRCS := $(S_DIR)/boids.c $(S_DIR)/vector.c #file-name.c

# generate names of object files
OBJS := $(SRCS:$(S_DIR)/%.c=$(B_DIR)/%.o)

# name of executable
EXEC := boids #name your executable file

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $(B_DIR)/$@ $(OBJS) $(CFLAGS)

# recipe for building object files
$(OBJS): $(@:%.c) $(HDRS) Makefile
	$(CC) -o $@ $(@:$(B_DIR)/%.o=$(S_DIR)/%.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(B_DIR)/$(EXEC) $(OBJS)

.PHONY: all clean