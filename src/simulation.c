#include "defs.h"

void *simulate_world(void *ptr)
{
  s_sim_data *sim_data = (s_sim_data*)ptr;
  
  while(!sim_data->quit)
  {
    if(sim_data->paused)
    {
      nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
      continue;
    }
    
    double t0 = glfwGetTime();
    world_simulate_frame(sim_data->world);
    double t1 = glfwGetTime();
    
    if(t1-t0 < 1.0/sim_data->fps_max && sim_data->fps_max >= 0)
    {
      nanosleep((const struct timespec[]){{0, (1.0/sim_data->fps_max - (t1-t0)) * 1000.0 * 1000.0 * 1000.0}}, NULL);
    }
    sim_data->fps = 1.0/(glfwGetTime()-t0);
  }
  
  return NULL;
}
