#include "defs.hpp"

int bot_print(Bot* bot) {
    assert(bot);
    fnn_print(&bot->fnn);
    std::cout << std::endl;

    return 0;
}
/*
typedef struct
{
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
} Part;

typedef struct
{
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
} Bot;
*/

int bot_copy(Bot* dest, Bot* src) {
    assert(dest);
    assert(src);

    dest->id = src->id;
    dest->age = src->age;
    dest->energy = src->energy;
    dest->health = src->health;
    dest->speed = src->speed;
    dest->turn_rate = src->turn_rate;

    // Brain
    int r = fnn_neurons_copy(&dest->fnn, &src->fnn);
    if (r != 0) {
        return -1;
    }

    dest->total_eyes = src->total_eyes;
    dest->total_spikes = src->total_spikes;
    dest->total_ears = src->total_ears;

    // Parts
    dest->num_parts = src->num_parts;
    for (int p = 0; p < dest->num_parts; ++p) {
        dest->parts[p] = src->parts[p];
    }

    return 0;
}

int bot_eye_add(Bot* bot, int part, float angle, float fov, float dist) {
    assert(bot);

    if (bot->parts[part].num_eyes >= MAX_EYES) {
        return -1;
    }

    int e = bot->parts[part].num_eyes;
    bot->parts[part].eyes[e].angle = angle;
    bot->parts[part].eyes[e].fov = fov;
    bot->parts[part].eyes[e].dist = dist;

    bot->parts[part].eyes[e].r = 0.0;
    bot->parts[part].eyes[e].g = 0.0;
    bot->parts[part].eyes[e].b = 0.0;

    bot->parts[part].num_eyes++;
    bot->total_eyes++;

    return 0;
}

int bot_spike_add(Bot* bot, int part, float length, float angle) {
    assert(bot);

    if (bot->parts[part].num_spikes >= MAX_SPIKES) {
        return -1;
    }

    int s = bot->parts[part].num_spikes;
    bot->parts[part].spikes[s].length = length;
    bot->parts[part].spikes[s].angle = angle;

    bot->parts[part].spikes[s].r = 0.8;
    bot->parts[part].spikes[s].g = 0.8;
    bot->parts[part].spikes[s].b = 0.8;

    bot->parts[part].num_spikes++;
    bot->total_spikes++;

    return 0;
}

int bot_ear_add(Bot* bot, int part, float dist) {
    assert(bot);

    if (bot->parts[part].num_ears >= MAX_EARS) {
        return -1;
    }

    int ear = bot->parts[part].num_ears;

    bot->parts[part].ears[ear].dist = dist;

    bot->parts[part].num_ears++;
    bot->total_ears++;

    return 0;
}
