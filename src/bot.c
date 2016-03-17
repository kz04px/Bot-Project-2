#include "defs.h"

int bot_eye_add(s_bot* bot, int part, float angle, float fov, float dist)
{
  assert(bot != NULL);
  
  if(bot->num_eyes >= MAX_EYES) {return -1;}
  
  int e = bot->num_eyes;
  bot->eyes[e].part = part;
  bot->eyes[e].angle = angle;
  bot->eyes[e].fov = fov;
  bot->eyes[e].dist = dist;
  
  bot->eyes[e].r = 0.0;
  bot->eyes[e].g = 0.0;
  bot->eyes[e].b = 0.0;
  
  bot->num_eyes++;
  
  return 0;
}

int bot_spike_add(s_bot* bot, int part, float length, float angle)
{
  assert(bot != NULL);
  
  if(bot->num_spikes >= MAX_SPIKES) {return -1;}
  
  int s = bot->num_spikes;
  bot->spikes[s].part = part;
  bot->spikes[s].length = length;
  bot->spikes[s].angle = angle;
  
  bot->spikes[s].r = 0.8;
  bot->spikes[s].g = 0.8;
  bot->spikes[s].b = 0.8;
  
  bot->num_spikes++;
  
  return 0;
}