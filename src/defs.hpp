#ifndef DEFS_HPP_INCLUDED
#define DEFS_HPP_INCLUDED

#include "world.hpp"

struct SimData {
    int fps_max;
    int fps;
    int paused;
    int quit;
    World *world;
};

extern float camera_x;
extern float camera_y;
extern float camera_zoom;
extern int window_width;
extern int window_height;
extern float window_ratio;
extern int window_fullscreen;
extern SimData sim_data;

#endif  // DEFS_H_INCLUDED
