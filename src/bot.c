#include "defs.h"

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