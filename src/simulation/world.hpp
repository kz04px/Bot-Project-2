#ifndef WORLD_HPP
#define WORLD_HPP

#include "bot.hpp"
#include "pellet.hpp"
#include "tile.hpp"

#define NUM_PARENTS 4
#define MAX_BOTS 128
#define MAX_PELLETS 512
#define GEN_MAX_FRAMES 5000

struct World {
    int seed;
    int frame;
    int generation;
    float w;
    float h;

    // Stats
    int pellets_eaten_good;
    int pellets_eaten_bad;
    float average_fitness;

    int grid_w;
    int grid_h;
    Tile **grid;

    int num_bots;
    Bot bots[MAX_BOTS];
    Bot parents[NUM_PARENTS];

    int num_pellets;
    Pellet pellets[MAX_PELLETS];
};

int world_simulate_frame(World *world);
int world_print_details(World *world);
int world_init(World *world);
int world_bot_add(World *world);
int world_bots_add(World *world, int n);
int world_pellet_add(World *world);
int world_pellet_remove(World *world, int p);
int world_pellets_add(World *world, int n);
int world_pellet_replace(World *world, int p);

#endif
