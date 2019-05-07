#ifndef DEFS_HPP_INCLUDED
#define DEFS_HPP_INCLUDED

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include "fnn.hpp"

// Visuals
#define EYE_CONE_ACCURACY 16
#define EAR_RANGE_ACCURACY 64

// Bots
#define BOT_START_ENERGY 500
#define BOT_START_HEALTH 500
#define MIN_VIEW_DIST 1.0
#define MAX_VIEW_DIST 5.0
#define MAX_PARTS 12
#define MAX_EYES 16
#define MAX_SPIKES 32
#define MAX_EARS 1

// Pellets
#define PELLET_ENERGY 200

// Simulation
#define NUM_PARENTS 4
#define MAX_BOTS 128
#define MAX_PELLETS 512
#define GEN_MAX_FRAMES 5000

#define DEG_TO_RAD(x) (x * 0.0174532925)
#define RAD_TO_DEG(x) (x / 0.0174532925)
#define RAND_BETWEEN(a, b) ((b - a) * ((float)rand() / RAND_MAX) + a)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

struct Pellet {
    int id;
    float x;
    float y;
    float size;
    int energy;
    float r;
    float g;
    float b;
};

struct Eye {
    float angle;
    float fov;
    float dist;
    // Inputs
    float str;
    float r;
    float g;
    float b;
};

struct Spike {
    float angle;
    float length;
    float r;
    float g;
    float b;
};

struct Ear {
    float dist;
    // Inputs
    float str;
};

struct Part {
    float radius;
    float angle;
    float x;
    float y;

    float r;
    float g;
    float b;

    int num_spikes;
    Spike spikes[MAX_SPIKES];

    int num_eyes;
    Eye eyes[MAX_EYES];

    int num_ears;
    Ear ears[MAX_EARS];
};

struct Bot {
    int id;
    int age;
    int energy;
    int health;
    float speed;
    float turn_rate;
    FNN fnn;

    int total_eyes;
    int total_spikes;
    int total_ears;

    int num_parts;
    Part parts[MAX_PARTS];
};

struct Tile {
    int food;

    float r;
    float g;
    float b;
};

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

struct SimData {
    int fps_max;
    int fps;
    int paused;
    int quit;
    World *world;
};

struct Buffers {
    GLuint vao, ibo, vbo, tbo, cbo, rbo;
};

extern float camera_x;
extern float camera_y;
extern float camera_zoom;
extern int window_width;
extern int window_height;
extern float window_ratio;
extern int window_fullscreen;
extern SimData sim_data;

// simulation.c
void *simulate_world(void *ptr);

// buffers.c
int buffers_init_background(Buffers *buffers);
int buffers_init_pellets(Buffers *buffers);
int buffers_init_bots(Buffers *buffers);

int buffers_fill_background(World *world, Buffers *buffers);
int buffers_fill_pellets(World *world, Buffers *buffers);
int buffers_fill_bots(World *world);

int buffer_fill_bot_cones(Part *part, Buffers *buffers);
int buffer_fill_bot_spikes(Part *part, Buffers *buffers);
int buffer_fill_bot_body(Part *part, Buffers *buffers, float scale);
int buffer_fill_bot_eye_borders(Part *part, Buffers *buffers);
int buffer_fill_bot_eyes(Part *part, Buffers *buffers);
int buffer_fill_bot_ears(Part *part, Buffers *buffers);

// bot.c
int bot_print(Bot *bot);
int bot_eye_add(Bot *bot, int part, float angle, float fov, float dist);
int bot_spike_add(Bot *bot, int part, float length, float angle);
int bot_ear_add(Bot *bot, int part, float dist);
int bot_copy(Bot *dest, Bot *src);

// world.c
int world_simulate_frame(World *world);
int world_print_details(World *world);
int world_init(World *world);
int world_bot_add(World *world);
int world_bots_add(World *world, int n);
int world_pellet_add(World *world);
int world_pellet_remove(World *world, int p);
int world_pellets_add(World *world, int n);
int world_pellet_replace(World *world, int p);

// io.c
int load_settings(const char *filename);
int print_log(const char *format, ...);
void print_log_shader_info(GLuint shader_index);
int screenshot_tga(char *path, int w, int h);

// callbacks.c
void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow *window,
                                   double xpos,
                                   double ypos);
void glfw_mouse_scroll_callback(GLFWwindow *window,
                                double xoffset,
                                double yoffset);
void glfw_keyboard_callback(GLFWwindow *window,
                            int key,
                            int scancode,
                            int action,
                            int mods);
void glfw_mouse_button_callback(GLFWwindow *window,
                                int button,
                                int action,
                                int mods);

// shaders.c
int create_shader(const char *filename, int type);

#endif  // DEFS_H_INCLUDED
