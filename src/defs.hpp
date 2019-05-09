#ifndef DEFS_HPP_INCLUDED
#define DEFS_HPP_INCLUDED

struct SimData {
    int fps_max;
    int fps;
    int paused;
    int quit;
};

extern float camera_x;
extern float camera_y;
extern float camera_zoom;
extern int window_fullscreen;
extern SimData sim_data;

#endif  // DEFS_H_INCLUDED
