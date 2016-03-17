#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define MAX_BOTS 64
#define MAX_PELLETS 256
#define EYE_CONE_ACCURACY 8

#define MAX_PARTS   12
#define MAX_EYES    16
#define MAX_SPIKES  32

#define DEG_TO_RAD(x) (x*(3.142/180.0))
#define RAND_BETWEEN(a, b) ((b-a)*((float)rand()/RAND_MAX)+a)

typedef struct
{
  int id;
  float x;
  float y;
  float size;
  int energy;
  
  float r;
  float g;
  float b;
} s_pellet;

typedef struct
{
  float radius;
  float angle;
  float x;
  float y;
  
  float r;
  float g;
  float b;
} s_part;

typedef struct
{
  int part;
  float angle;
  float fov;
  float dist;
  
  float str;
  float r;
  float g;
  float b;
} s_eye;

typedef struct
{
  int part;
  float angle;
  float length;
  
  float r;
  float g;
  float b;
} s_spike;

typedef struct
{
  int id;
  int energy;
  float speed;
  float turn_rate;
  
  int num_spikes;
  s_spike spikes[MAX_SPIKES];
  
  int num_eyes;
  s_eye eyes[MAX_EYES];
  
  int num_parts;
  s_part parts[MAX_PARTS];
} s_bot;

typedef struct
{
  int food;
  
  float r;
  float g;
  float b;
} s_tile;

typedef struct
{
  int seed;
  float w;
  float h;
  
  int grid_w;
  int grid_h;
  s_tile **grid;
  
  int num_bots;
  s_bot bots[MAX_BOTS];
  
  int num_pellets;
  s_pellet pellets[MAX_PELLETS];
  
} s_world;

typedef struct
{
  GLuint vao, ibo, vbo, tbo, cbo, rbo;
} s_buffers;

float camera_x;
float camera_y;
float camera_zoom;
int window_width;
int window_height;
float window_ratio;
int window_fullscreen;

// buffers.c
int buffers_init_background(s_buffers* buffers);
int buffers_init_pellets(s_buffers* buffers);
int buffers_init_bots(s_buffers* buffers);

int buffers_fill_background(s_world* world, s_buffers* buffers);
int buffers_fill_pellets(s_world* world, s_buffers* buffers);
int buffers_fill_bots(s_world* world);

// bot.c
int bot_eye_add(s_bot* bot, int part, float angle, float fov, float dist);
int bot_spike_add(s_bot* bot, int part, float length, float angle);

// world.c
int world_simulate_frame(s_world *world);
int world_print_details(s_world *world);
int world_init(s_world *world);
int world_bot_add(s_world *world);
int world_bots_add(s_world *world, int n);
int world_pellet_add(s_world *world);
int world_pellet_remove(s_world *world, int p);
int world_pellets_add(s_world *world, int n);

// io.c
int load_settings(const char *filename);
int print_log(const char *format, ...);
void print_log_shader_info(GLuint shader_index);
int screenshot_tga(char* path, int w, int h);

// callbacks.c
void glfw_window_size_callback(GLFWwindow* window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// shaders.c
int create_shader(const char* filename, int type);

#endif // DEFS_H_INCLUDED
