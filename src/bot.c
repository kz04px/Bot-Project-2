#include "defs.h"

int bot_print(s_bot *bot)
{
  assert(bot != NULL);
  /*
  printf("id: %i\n", bot->id);
  printf("x: %.2f\n", bot->parts[0].x);
  printf("y: %.2f\n", bot->parts[0].y);
  printf("RGB(%f, %f, %f)\n", bot->parts[0].r, bot->parts[0].g, bot->parts[0].b);
  printf("age: %i\n", bot->age);
  printf("energy: %i\n", bot->energy);
  printf("health: %i\n", bot->health);
  printf("speed: %f\n", bot->speed);
  printf("turn_rate: %f\n", bot->turn_rate);
  printf("total_eyes: %i\n", bot->total_eyes);
  printf("total_spikes: %i\n", bot->total_spikes);
  printf("total_ears: %i\n", bot->total_ears);
  printf("num_parts: %i\n", bot->num_parts);
  printf("brain: %p\n", &bot->fnn);
  */
  fnn_print(&bot->fnn);
  printf("\n");
  
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
  s_spike spikes[MAX_SPIKES];
  
  int num_eyes;
  s_eye eyes[MAX_EYES];
  
  int num_ears;
  s_ear ears[MAX_EARS];
} s_part;

typedef struct
{
  int id;
  int age;
  int energy;
  int health;
  float speed;
  float turn_rate;
  s_fnn fnn;
  
  int total_eyes;
  int total_spikes;
  int total_ears;
  
  int num_parts;
  s_part parts[MAX_PARTS];
} s_bot;
*/

int bot_copy(s_bot* dest, s_bot* src)
{
  assert(dest != NULL);
  assert(src != NULL);
  
  dest->id = src->id;
  dest->age = src->age;
  dest->energy = src->energy;
  dest->health = src->health;
  dest->speed = src->speed;
  dest->turn_rate = src->turn_rate;
  
  // Brain
  int r;
  r = fnn_neurons_copy(&dest->fnn, &src->fnn);
  if(r != 0) {return -1;}
  
  dest->total_eyes = src->total_eyes;
  dest->total_spikes = src->total_spikes;
  dest->total_ears = src->total_ears;
  
  // Parts
  dest->num_parts = src->num_parts;
  int p;
  for(p = 0; p < dest->num_parts; ++p)
  {
    dest->parts[p] = src->parts[p];
  }
  
  return 0;
}

int bot_eye_add(s_bot* bot, int part, float angle, float fov, float dist)
{
  assert(bot != NULL);
  
  if(bot->parts[part].num_eyes >= MAX_EYES) {return -1;}
  
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

int bot_spike_add(s_bot* bot, int part, float length, float angle)
{
  assert(bot != NULL);
  
  if(bot->parts[part].num_spikes >= MAX_SPIKES) {return -1;}
  
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

int bot_ear_add(s_bot* bot, int part, float dist)
{
  assert(bot != NULL);
  
  if(bot->parts[part].num_ears >= MAX_EARS) {return -1;}
  
  int ear = bot->parts[part].num_ears;
  
  bot->parts[part].ears[ear].dist = dist;
  
  bot->parts[part].num_ears++;
  bot->total_ears++;
  
  return 0;
}
