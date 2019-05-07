#ifndef BUFFERS_HPP
#define BUFFERS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "bot.hpp"
#include "world.hpp"

#define EYE_CONE_ACCURACY 16
#define EAR_RANGE_ACCURACY 64

struct Buffers {
    GLuint vao, ibo, vbo, tbo, cbo, rbo;
};

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

#endif
