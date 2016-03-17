#include "defs.h"

int world_simulate_frame(s_world *world)
{
  assert(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    world->bots[b].parts[0].angle += world->bots[b].turn_rate*RAND_BETWEEN(-1.0, 1.0);
    world->bots[b].parts[0].x += world->bots[b].speed*sin(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].y += world->bots[b].speed*cos(world->bots[b].parts[0].angle);
    
    float spacer = 0.3;
    
    // Update tail positions
    int i;
    for(i = 1; i < world->bots[b].num_parts; ++i)
    {
      float dx = world->bots[b].parts[i-1].x - world->bots[b].parts[i].x;
      float dy = world->bots[b].parts[i-1].y - world->bots[b].parts[i].y;
      float angle = atan2(dx, dy);
      
      world->bots[b].parts[i].angle = angle;
      world->bots[b].parts[i].x = world->bots[b].parts[i-1].x - spacer*sin(world->bots[b].parts[i].angle);
      world->bots[b].parts[i].y = world->bots[b].parts[i-1].y - spacer*cos(world->bots[b].parts[i].angle);
      
      spacer *= 0.9;
    }
    
    // Eat pellets
    int p;
    for(p = 0; p < world->num_pellets; ++p)
    {
      float dx = world->bots[b].parts[0].x - world->pellets[p].x;
      float dy = world->bots[b].parts[0].y - world->pellets[p].y;
      
      if(fabs(dx) > 10.0) {continue;}
      if(fabs(dy) > 10.0) {continue;}
      
      float dist = sqrt(dx*dx + dy*dy);
      
      if(dist > 0.25) {continue;}
      
      world->bots[b].energy += world->pellets[p].energy;
      world_pellet_remove(world, p);
      world_pellet_add(world);
    }
    
    // Edge detection
    if(world->bots[b].parts[0].x < 0.0)            {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].x += world->w;}}
    else if(world->bots[b].parts[0].x >= world->w) {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].x -= world->w;}}
    if(world->bots[b].parts[0].y < 0.0)            {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].y += world->h;}}
    else if(world->bots[b].parts[0].y >= world->h) {for(i = 0; i < world->bots[b].num_parts; ++i) {world->bots[b].parts[i].y -= world->h;}}
  }
  
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
  
  float spacer = 0.3;
  
  world->bots[b].id = id;
  world->bots[b].energy = 100;
  world->bots[b].speed = 0.01;
  world->bots[b].turn_rate = 0.1;
  world->bots[b].num_eyes = 0;
  world->bots[b].num_spikes = 0;
  world->bots[b].num_parts = MAX_PARTS - rand()%3;
  
  assert(world->bots[b].num_parts <= MAX_PARTS);
  assert(world->bots[b].num_parts >= 0);
  
  // Body parts
  for(i = 0; i < world->bots[b].num_parts; ++i)
  {
    if(i == 0)
    {
      world->bots[b].parts[0].radius = spacer;
      world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2*M_PI);
      world->bots[b].parts[0].x = x;
      world->bots[b].parts[0].y = y;
    }
    else
    {
      world->bots[b].parts[i].angle = world->bots[b].parts[i-1].angle + RAND_BETWEEN(-0.5, 0.5); //RAND_BETWEEN(0.0, 2*M_PI);
      world->bots[b].parts[i].x = world->bots[b].parts[i-1].x - spacer*sin(world->bots[b].parts[i].angle);
      world->bots[b].parts[i].y = world->bots[b].parts[i-1].y - spacer*cos(world->bots[b].parts[i].angle);
      
      spacer *= 0.9;
    }
    
    world->bots[b].parts[i].r = red - (float)i/world->bots[b].num_parts;
    world->bots[b].parts[i].g = green - (float)i/world->bots[b].num_parts;
    world->bots[b].parts[i].b = blue - (float)i/world->bots[b].num_parts;
  }
  
  // Eyes
  world->bots[b].num_eyes = 2;
  float eye_total_angle = 2.0*M_PI/6;
  int e;
  for(e = 0; e < world->bots[b].num_eyes; ++e)
  {
    world->bots[b].eyes[e].part = 0;
    world->bots[b].eyes[e].angle = (e - 0.5*(world->bots[b].num_eyes-1))*eye_total_angle;
    world->bots[b].eyes[e].fov = 2.0*M_PI/5;
    world->bots[b].eyes[e].dist = 3.0;
    
    world->bots[b].eyes[e].r = 1.0;
    world->bots[b].eyes[e].g = 0.0;
    world->bots[b].eyes[e].b = 1.0;
  }
  
  // Spikes
  bot_spike_add(&world->bots[b], 0, 0.5, 0.0);
  
  bot_spike_add(&world->bots[b], 1, 0.4, 5*(2.0*M_PI/6) );
  bot_spike_add(&world->bots[b], 1, 0.4, 1*(2.0*M_PI/6) );
  
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 0.25, 3*(2.0*M_PI/7) );
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 0.25, 4*(2.0*M_PI/7) );
  
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
  world->w = 60.0;
  world->h = 30.0;
  world->num_bots = 0;
  world->num_pellets = 0;
  world->grid_w = 20;
  world->grid_h = 10;
  
  //world->grid = malloc(world->grid_w * world->grid_h * sizeof(s_tile));
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