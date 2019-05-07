#ifndef BOT_HPP
#define BOT_HPP

#include "fnn.hpp"

#define BOT_START_ENERGY 500
#define BOT_START_HEALTH 500
#define MIN_VIEW_DIST 1.0
#define MAX_VIEW_DIST 5.0
#define MAX_PARTS 12
#define MAX_EYES 16
#define MAX_SPIKES 32
#define MAX_EARS 1

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

int bot_print(Bot *bot);
int bot_eye_add(Bot *bot, int part, float angle, float fov, float dist);
int bot_spike_add(Bot *bot, int part, float length, float angle);
int bot_ear_add(Bot *bot, int part, float dist);
int bot_copy(Bot *dest, Bot *src);

#endif
