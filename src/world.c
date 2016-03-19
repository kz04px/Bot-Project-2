#include "defs.h"

#define NORMALISE_RAD(x) (x >= 2.0*M_PI) ? x=x-2.0*M_PI : ((x < 0) ? x=x+2.0*M_PI : x)
const float wiggle[32] = {0.000000, 0.195090, 0.382683, 0.555570, 0.707107, 0.831470, 0.923880, 0.980785, 1.000000, 0.980785, 0.923880, 0.831470, 0.707107, 0.555570,0.382683, 0.195090, 0.000000, -0.195090, -0.382683, -0.555570, -0.707107, -0.831470, -0.923880, -0.980785, -1.000000, -0.980785, -0.923880, -0.831470, -0.707107, -0.555570, -0.382683, 0.195090};

float angle_difference(float x, float y)
{
  float dif = x-y;
  NORMALISE_RAD(dif);
  if(dif > M_PI)
  {
    return (2.0*M_PI - dif);
  }
  else
  {
    return dif;
  }
}

int world_simulate_frame(s_world *world)
{
  assert(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    int p;
    int e;
    int i;
    int ear;
    int part;
    
    // Reset inputs
    for(part = 0; part < world->bots[b].num_parts; ++part)
    {
      // Ears
      for(ear = 0; ear < world->bots[b].parts[part].num_ears; ++ear)
      {
        world->bots[b].parts[part].ears[ear].str = 0.0;
      }
      
      // Eyes
      for(e = 0; e < world->bots[b].parts[part].num_eyes; ++e)
      {
        world->bots[b].parts[part].eyes[e].str = 0.0;
        world->bots[b].parts[part].eyes[e].r = 0.0;
        world->bots[b].parts[part].eyes[e].g = 0.0;
        world->bots[b].parts[part].eyes[e].b = 0.0;
      }
    }
    
    // Eating
    for(p = 0; p < world->num_pellets; ++p)
    {
      float dx = world->bots[b].parts[0].x - world->pellets[p].x;
      if(fabs(dx) > world->bots[b].parts[0].radius) {continue;}
      
      float dy = world->bots[b].parts[0].y - world->pellets[p].y;
      if(fabs(dy) > world->bots[b].parts[0].radius) {continue;}
        
      float dist = sqrt(dx*dx + dy*dy);
      
      if(dist <= world->bots[b].parts[0].radius)
      {
        world->bots[b].energy += world->pellets[p].energy;
        world_pellet_remove(world, p);
        world_pellet_add(world);
        p--;
        continue;
      }
    }
    
    for(part = 0; part < world->bots[b].num_parts; ++part)
    {
      // Skip looking if body part has no eyes
      if(world->bots[b].parts[part].num_eyes == 0 && world->bots[b].parts[part].num_ears == 0) {continue;}
      
      // Pellets
      for(p = 0; p < world->num_pellets; ++p)
      {
        float dx = world->bots[b].parts[part].x - world->pellets[p].x;
        if(fabs(dx) > MAX_VIEW_DIST) {continue;}
        
        float dy = world->bots[b].parts[part].y - world->pellets[p].y;
        if(fabs(dy) > MAX_VIEW_DIST) {continue;}
        
        float dist = sqrt(dx*dx + dy*dy);
        if(dist > MAX_VIEW_DIST) {continue;}
        
        float angle = atan2(dx, dy) + M_PI;
        
        // Eyes
        for(e = 0; e < world->bots[b].parts[part].num_eyes; ++e)
        {
          float absolute_angle = world->bots[b].parts[part].angle + world->bots[b].parts[part].eyes[e].angle;
          NORMALISE_RAD(absolute_angle);
          
          float angle_dif = angle_difference(angle, absolute_angle);
          NORMALISE_RAD(angle_dif);
          NORMALISE_RAD(angle_dif);
          NORMALISE_RAD(angle_dif);
          
          if(angle_dif > world->bots[b].parts[part].eyes[e].fov/2) {continue;}
          if(1.0 - dist/world->bots[b].parts[part].eyes[e].dist < world->bots[b].parts[part].eyes[e].str) {continue;}
          
          world->bots[b].parts[part].eyes[e].str = 1.0 - dist/world->bots[b].parts[part].eyes[e].dist;
          world->bots[b].parts[part].eyes[e].r = world->pellets[p].r;
          world->bots[b].parts[part].eyes[e].g = world->pellets[p].g;
          world->bots[b].parts[part].eyes[e].b = world->pellets[p].b;
        }
      }
      
      // Bots
      int b2;
      for(b2 = 0; b2 < world->num_bots; ++b2)
      {
        if(b == b2) {continue;}
        
        float dx_test = world->bots[b].parts[0].x - world->bots[b2].parts[0].x;
        if(fabs(dx_test) > 2*MAX_VIEW_DIST) {continue;}
        
        float dy_test = world->bots[b].parts[0].y - world->bots[b2].parts[0].y;
        if(fabs(dy_test) > 2*MAX_VIEW_DIST) {continue;}
        
        float dx = world->bots[b].parts[part].x - world->bots[b2].parts[0].x;
        float dy = world->bots[b].parts[part].y - world->bots[b2].parts[0].y;
        float dist = sqrt(dx*dx + dy*dy);
        
        float dx_last = world->bots[b].parts[part].x - world->bots[b2].parts[world->bots[b2].num_parts-1].x;
        float dy_last = world->bots[b].parts[part].y - world->bots[b2].parts[world->bots[b2].num_parts-1].y;
        float dist_last = sqrt(dx_last*dx_last + dy_last*dy_last);
        
        if(dist > dist_last)
        {
          dist = dist_last;
          dx = dx_last;
          dy = dy_last;
        }
        
        float angle = atan2(dx, dy) + M_PI;
        
        // Ears
        for(ear = 0; ear < world->bots[b].parts[part].num_ears; ++ear)
        {
          if(dist > world->bots[b].parts[part].ears[ear].dist) {continue;}
          
          if(1.0 - dist/world->bots[b].parts[part].ears[ear].dist < world->bots[b].parts[part].ears[ear].str) {continue;}
          
          world->bots[b].parts[part].ears[ear].str = 1.0 - dist/world->bots[b].parts[part].ears[ear].dist;
        }
        
        // Eyes
        for(e = 0; e < world->bots[b].parts[part].num_eyes; ++e)
        {
          if(dist > world->bots[b].parts[part].eyes[e].dist) {continue;}
          
          float absolute_angle = world->bots[b].parts[part].angle + world->bots[b].parts[part].eyes[e].angle;
          NORMALISE_RAD(absolute_angle);
          
          float angle_dif = angle_difference(angle, absolute_angle);
          NORMALISE_RAD(angle_dif);
          NORMALISE_RAD(angle_dif);
          NORMALISE_RAD(angle_dif);
          
          if(angle_dif > world->bots[b].parts[part].eyes[e].fov/2) {continue;}
          if(1.0 - dist/world->bots[b].parts[part].eyes[e].dist < world->bots[b].parts[part].eyes[e].str) {continue;}
          
          world->bots[b].parts[part].eyes[e].str = 1.0 - dist/world->bots[b].parts[part].eyes[e].dist;
          world->bots[b].parts[part].eyes[e].r = world->bots[b2].parts[0].r;
          world->bots[b].parts[part].eyes[e].g = world->bots[b2].parts[0].g;
          world->bots[b].parts[part].eyes[e].b = world->bots[b2].parts[0].b;
        }
      }
    }
    
    // Random movement
    //world->bots[b].parts[0].angle += world->bots[b].turn_rate*RAND_BETWEEN(-1.0, 1.0);
    
    // Basic movement
         if(world->bots[b].parts[0].eyes[0].str > world->bots[b].parts[0].eyes[1].str) {world->bots[b].parts[0].angle += world->bots[b].turn_rate;}
    else if(world->bots[b].parts[0].eyes[0].str < world->bots[b].parts[0].eyes[1].str) {world->bots[b].parts[0].angle -= world->bots[b].turn_rate;}
    
    NORMALISE_RAD(world->bots[b].parts[0].angle);
    
    world->bots[b].parts[0].x += world->bots[b].speed*sin(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].y += world->bots[b].speed*cos(world->bots[b].parts[0].angle);
    
    // Update tail positions
    float spacer = 0.3;
    for(i = 1; i < world->bots[b].num_parts; ++i)
    {
      float dx = world->bots[b].parts[i-1].x - world->bots[b].parts[i].x;
      float dy = world->bots[b].parts[i-1].y - world->bots[b].parts[i].y;
      float angle = atan2(dx, dy);
      
      world->bots[b].parts[i].angle = angle + 0.01*wiggle[(world->frame/8)%32];
      
      NORMALISE_RAD(world->bots[b].parts[i].angle);
      
      world->bots[b].parts[i].x = world->bots[b].parts[i-1].x - spacer*sin(world->bots[b].parts[i].angle);
      world->bots[b].parts[i].y = world->bots[b].parts[i-1].y - spacer*cos(world->bots[b].parts[i].angle);
      
      spacer *= 0.9;
    }
    
    // Edge detection
    if(world->bots[b].parts[0].x < 0.0)            {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].x += world->w;}}
    else if(world->bots[b].parts[0].x >= world->w) {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].x -= world->w;}}
    if(world->bots[b].parts[0].y < 0.0)            {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].y += world->h;}}
    else if(world->bots[b].parts[0].y >= world->h) {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].y -= world->h;}}
  }
  
  world->frame++;
  
  return 0;
}

int world_bots_add(s_world *world, int n)
{
  assert(world != NULL);
  
  int i;
  for(i = 0; i < n; ++i)
  {
    int r = world_bot_add(world);
    if(r != 0) {return r;}
  }
  
  return 0;
}

int world_bot_add(s_world *world)
{
  assert(world != NULL);
  
  if(world->num_bots >= MAX_BOTS) {return -1;}
  
  int i;
  int b = world->num_bots;
  
  static int id = 0;
  
  float x = RAND_BETWEEN(0.0, world->w);
  float y = RAND_BETWEEN(0.0, world->h);
  float red = RAND_BETWEEN(0.0, 1.0);
  float green = RAND_BETWEEN(0.0, 1.0);
  float blue = RAND_BETWEEN(0.0, 1.0);
  
  float spacer = 0.25;
  
  world->bots[b].id = id;
  world->bots[b].energy = 100;
  world->bots[b].health = 100;
  world->bots[b].speed = 0.01;
  world->bots[b].turn_rate = 0.01;
  world->bots[b].num_parts = MAX_PARTS - rand()%3;
  world->bots[b].total_eyes = 0;
  world->bots[b].total_spikes = 0;
  world->bots[b].total_ears = 0;
  
  assert(world->bots[b].num_parts <= MAX_PARTS);
  assert(world->bots[b].num_parts >= 0);
  
  // Body parts
  int part;
  for(part = 0; part < world->bots[b].num_parts; ++part)
  {
    if(part == 0)
    {
      world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2*M_PI);
      world->bots[b].parts[0].x = x;
      world->bots[b].parts[0].y = y;
    }
    else
    {
      world->bots[b].parts[part].angle = world->bots[b].parts[part-1].angle + RAND_BETWEEN(-0.5, 0.5); //RAND_BETWEEN(0.0, 2*M_PI);
      world->bots[b].parts[part].x = world->bots[b].parts[part-1].x - spacer*sin(world->bots[b].parts[part].angle);
      world->bots[b].parts[part].y = world->bots[b].parts[part-1].y - spacer*cos(world->bots[b].parts[part].angle);
      
      spacer *= 0.9;
    }
    
    world->bots[b].parts[part].num_eyes = 0;
    world->bots[b].parts[part].num_spikes = 0;
    world->bots[b].parts[part].num_ears = 0;
    world->bots[b].parts[part].radius = spacer;
    world->bots[b].parts[part].r = red - (float)part/world->bots[b].num_parts;
    world->bots[b].parts[part].g = green - (float)part/world->bots[b].num_parts;
    world->bots[b].parts[part].b = blue - (float)part/world->bots[b].num_parts;
  }
  
  // Eyes
  bot_eye_add(&world->bots[b], 0, 1*(2.0*M_PI/12), 2.0*M_PI/5, 3.0);
  bot_eye_add(&world->bots[b], 0, 11*(2.0*M_PI/12), 2.0*M_PI/5, 3.0);
  //bot_eye_add(&world->bots[b], 1, 1*(2.0*M_PI/4), 2.0*M_PI/4, 1.0);
  //bot_eye_add(&world->bots[b], 1, 3*(2.0*M_PI/4), 2.0*M_PI/4, 1.0);
  bot_eye_add(&world->bots[b], world->bots[b].num_parts-1, M_PI, 2.0*M_PI/4, 1.5);
  
  // Spikes (Caterpillar look)
  bot_spike_add(&world->bots[b], 0, 0.5, 0.0);
  bot_spike_add(&world->bots[b], 0, 0.4, 5*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 0, 0.4, 1*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 1, 0.4, 5*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 1, 0.4, 1*(2.0*M_PI/6));
  for(i = 1; i < world->bots[b].num_parts-1; ++i)
  {
    bot_spike_add(&world->bots[b], i, 1.5*world->bots[b].parts[i].radius, 4*(2.0*M_PI/6));
    bot_spike_add(&world->bots[b], i, 1.5*world->bots[b].parts[i].radius, 2*(2.0*M_PI/6));
  }
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 3.0*world->bots[b].parts[world->bots[b].num_parts-1].radius, 4*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 3.0*world->bots[b].parts[world->bots[b].num_parts-1].radius, 2*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 5.0*world->bots[b].parts[world->bots[b].num_parts-1].radius, 2.0*M_PI/2);
  
  // Ears
  bot_ear_add(&world->bots[b], 0, 5.0);
  
  world->num_bots++;
  id++;
  
  return 0;
}

int world_pellet_remove(s_world *world, int p)
{
  assert(world != NULL);
  
  if(p >= world->num_pellets) {return -1;}
  
  world->pellets[p] = world->pellets[world->num_pellets-1];
  world->num_pellets--;
  
  return 0;
}

int world_pellets_add(s_world *world, int n)
{
  assert(world != NULL);
  
  int i;
  for(i = 0; i < n; ++i)
  {
    int r = world_pellet_add(world);
    if(r != 0) {return r;}
  }
  
  return 0;
}

int world_pellet_add(s_world *world)
{
  assert(world != NULL);
  
  if(world->num_pellets >= MAX_PELLETS) {return -1;}
  
  int i = world->num_pellets;
  
  static int id = 0;
  
  world->pellets[i].id = id;
  
  world->pellets[i].x = RAND_BETWEEN(0, world->w);
  world->pellets[i].y = RAND_BETWEEN(0, world->h);
  world->pellets[i].size = 0.2;
  world->pellets[i].energy = 100;
  
  world->pellets[i].r = 1.0;
  world->pellets[i].g = 1.0;
  world->pellets[i].b = 0.0;
  
  world->num_pellets++;
  id++;
  
  return 0;
}

int world_print_details(s_world *world)
{
  assert(world != NULL);
  
  printf("--- World ---\n");
  printf("Seed: %i\n", world->seed);
  printf("Frame: %i\n", world->frame);
  printf("Width: %f\n", world->w);
  printf("Height: %f\n", world->h);
  printf("Num bots: %i\n", world->num_bots);
  printf("Num pellets: %i\n", world->num_pellets);
  printf("Grid w: %i\n", world->grid_w);
  printf("Grid h: %i\n", world->grid_h);
  printf("\n");
  
  return 0;
}

int world_init(s_world *world)
{
  assert(world != NULL);
  
  world->seed = time(0);
  world->frame = 0;
  world->w = 120.0;
  world->h = 60.0;
  world->num_bots = 0;
  world->num_pellets = 0;
  world->grid_w = 20;
  world->grid_h = 10;
  
  srand(world->seed);
  world->grid = malloc(world->grid_w * sizeof(s_tile*));
  
  int w;
  for(w = 0; w < world->grid_w; ++w)
  {
    world->grid[w] = malloc(world->grid_h * sizeof(s_tile));
    int h;
    for(h = 0; h < world->grid_h; ++h)
    {
      world->grid[w][h].food = 100;
      world->grid[w][h].r = RAND_BETWEEN(0.2, 0.3);
      world->grid[w][h].g = RAND_BETWEEN(0.4, 0.7);
      world->grid[w][h].b = RAND_BETWEEN(0.1, 0.2);
    }
  }
  
  return 0;
}