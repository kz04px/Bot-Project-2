#include "defs.h"

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
