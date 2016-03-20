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
  
  int num_bots_alive = 0;
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    if(world->bots[b].health <= 0) {continue;}
    num_bots_alive++;
    
    int p;
    int e;
    int i;
    int s;
    int ear;
    int part;
    
    // Reset inputs
    for(part = 0; part < world->bots[b].num_parts; ++part)
    {
      // Spikes
      for(s = 0; s < world->bots[b].parts[part].num_spikes; ++s)
      {
        world->bots[b].parts[part].spikes[s].r -= 0.005*(world->bots[b].parts[part].spikes[s].r - 0.8);
        world->bots[b].parts[part].spikes[s].g -= 0.005*(world->bots[b].parts[part].spikes[s].g - 0.8);
        world->bots[b].parts[part].spikes[s].b -= 0.005*(world->bots[b].parts[part].spikes[s].b - 0.8);
      }
      
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
      // Skip the body part has no eyes or ears
      if(world->bots[b].parts[part].num_eyes == 0 &&
         world->bots[b].parts[part].num_ears == 0 &&
         world->bots[b].parts[part].num_spikes == 0) {continue;}
      
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
        if(b == b2 || world->bots[b2].health <= 0) {continue;}
        
        float dx_test = world->bots[b].parts[0].x - world->bots[b2].parts[0].x;
        if(fabs(dx_test) > 2*MAX_VIEW_DIST) {continue;}
        
        float dy_test = world->bots[b].parts[0].y - world->bots[b2].parts[0].y;
        if(fabs(dy_test) > 2*MAX_VIEW_DIST) {continue;}
        
        int part2;
        for(part2 = 0; part2 < world->bots[b2].num_parts; ++part2)
        {
          float dx = world->bots[b].parts[part].x - world->bots[b2].parts[part2].x;
          float dy = world->bots[b].parts[part].y - world->bots[b2].parts[part2].y;
          float dist = sqrt(dx*dx + dy*dy);
          float angle = atan2(dx, dy) + M_PI;
        
          // Spikes
          for(s = 0; s < world->bots[b].parts[part].num_spikes; ++s)
          {
            if(dist > world->bots[b].parts[part].spikes[s].length + world->bots[b2].parts[part2].radius) {continue;}
            
            if(dist > world->bots[b2].parts[part2].radius)
            {
              float absolute_angle = world->bots[b].parts[part].angle + world->bots[b].parts[part].spikes[s].angle;
              NORMALISE_RAD(absolute_angle);
              
              float angle_dif = angle_difference(angle, absolute_angle);
              NORMALISE_RAD(angle_dif);
              NORMALISE_RAD(angle_dif);
              NORMALISE_RAD(angle_dif);
              
              if(angle_dif > 2.0*M_PI/32) {continue;}
            }
            
            world->bots[b2].health--;
            world->bots[b].energy++;
            world->bots[b].parts[part].spikes[s].r = 1.0;
            world->bots[b].parts[part].spikes[s].g = 0.0;
            world->bots[b].parts[part].spikes[s].b = 0.0;
          }
          
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
    }
    
    // Set brain inputs
    int pos = 0;
    for(part = 0; part < world->bots[b].num_parts; ++part)
    {
      int e;
      for(e = 0; e < world->bots[b].parts[part].num_eyes; ++e)
      {
        world->bots[b].fnn.neurons[0][4*pos + 0].in = world->bots[b].parts[part].eyes[e].str;
        world->bots[b].fnn.neurons[0][4*pos + 1].in = world->bots[b].parts[part].eyes[e].r;
        world->bots[b].fnn.neurons[0][4*pos + 2].in = world->bots[b].parts[part].eyes[e].g;
        world->bots[b].fnn.neurons[0][4*pos + 3].in = world->bots[b].parts[part].eyes[e].b;
        pos++;
      }
    }
    
    // Update brain
    fnn_feedforwards(&world->bots[b].fnn);
    
    // Random movement
    //world->bots[b].parts[0].angle += world->bots[b].turn_rate*RAND_BETWEEN(-1.0, 1.0);
    
    // Basic movement
    /*
         if(world->bots[b].parts[0].eyes[0].str > world->bots[b].parts[0].eyes[1].str) {world->bots[b].parts[0].angle += world->bots[b].turn_rate;}
    else if(world->bots[b].parts[0].eyes[0].str < world->bots[b].parts[0].eyes[1].str) {world->bots[b].parts[0].angle -= world->bots[b].turn_rate;}
    NORMALISE_RAD(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].x += world->bots[b].speed*sin(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].y += world->bots[b].speed*cos(world->bots[b].parts[0].angle);
    */
    
    // FNN movement
    float output_0 = world->bots[b].fnn.neurons[world->bots[b].fnn.num_layers-1][0].out;
    float output_1 = world->bots[b].fnn.neurons[world->bots[b].fnn.num_layers-1][1].out;
    
    world->bots[b].parts[0].x += output_0*world->bots[b].speed*sin(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].y += output_0*world->bots[b].speed*cos(world->bots[b].parts[0].angle);
    world->bots[b].parts[0].angle += (output_1 - 0.5)*world->bots[b].turn_rate;
    
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
    
    
    world->bots[b].age++;
    if(world->bots[b].energy > 0)
    {
      world->bots[b].energy--;
    }
    else if(world->bots[b].health > 0)
    {
      world->bots[b].health--;
    }
  }
  
  world->frame++;
    
  
  // Create the next generation
  #define NUM_PARENTS 4
  if(num_bots_alive <= NUM_PARENTS || world->frame >= GEN_MAX_FRAMES)
  {
    s_bot parents[NUM_PARENTS];
    int bot_number[world->num_bots];
    int bot_score[world->num_bots];
    
    // Score the bots
    for(b = 0; b < world->num_bots; ++b)
    {
      bot_number[b] = b;
      bot_score[b] = world->bots[b].age;
    }
    
    // Sort
    int a;
    for(a = 0; a < world->num_bots-1; ++a)
    {
      int b = a+1;
      for(b = a+1; b < world->num_bots; ++b)
      {
        if(bot_score[b] > bot_score[a])
        {
          int store;
          
          store = bot_score[a];
          bot_score[a] = bot_score[b];
          bot_score[b] = store;
          
          store = bot_number[a];
          bot_number[a] = bot_number[b];
          bot_number[b] = store;
        }
      }
    }
    
    // Create parents
    for(b = 0; b < NUM_PARENTS; ++b)
    {
      parents[b] = world->bots[bot_number[b]];
    }
    
    // Create new generation
    for(b = 0; b < world->num_bots; ++b)
    {
      world->bots[b] = parents[rand()%NUM_PARENTS];
      fnn_weights_jiggle(&world->bots[b].fnn);
      
      world->bots[b].energy = BOT_START_ENERGY;
      world->bots[b].health = BOT_START_HEALTH;
      
      float red = RAND_BETWEEN(0.0, 1.0);
      float green = RAND_BETWEEN(0.0, 1.0);
      float blue = RAND_BETWEEN(0.0, 1.0);
      int part;
      for(part = 0; part < world->bots[b].num_parts; ++part)
      {
        world->bots[b].parts[part].r = red - (float)part/world->bots[b].num_parts;
        world->bots[b].parts[part].g = green - (float)part/world->bots[b].num_parts;
        world->bots[b].parts[part].b = blue - (float)part/world->bots[b].num_parts;
      }
      
      world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2.0*M_PI);
      world->bots[b].parts[0].x = RAND_BETWEEN(0.0, world->w);
      world->bots[b].parts[0].y = RAND_BETWEEN(0.0, world->h);
    }
    
    world->frame = 0;
    world->generation++;
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
  
  float spacer = 0.25;
  world->bots[b].id = id;
  world->bots[b].age = 0;
  world->bots[b].energy = BOT_START_ENERGY;
  world->bots[b].health = BOT_START_HEALTH;
  world->bots[b].speed = 0.1*RAND_BETWEEN(0.8, 1.0);
  world->bots[b].turn_rate = 0.01*RAND_BETWEEN(0.8, 1.0);
  world->bots[b].num_parts = MAX_PARTS - rand()%3;
  world->bots[b].total_eyes = 0;
  world->bots[b].total_spikes = 0;
  world->bots[b].total_ears = 0;
  
  // Neural network
  int sizes[4] = {12, 2, 2, 2};
  fnn_init(&world->bots[b].fnn, 4, sizes);
  fnn_weights_randomise(&world->bots[b].fnn);
  
  assert(world->bots[b].num_parts <= MAX_PARTS);
  assert(world->bots[b].num_parts >= 0);
  
  // Body parts
  int part;
  for(part = 0; part < world->bots[b].num_parts; ++part)
  {
    if(part == 0)
    {
      world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2.0*M_PI);
      world->bots[b].parts[0].x = x;
      world->bots[b].parts[0].y = y;
    }
    else
    {
      world->bots[b].parts[part].angle = world->bots[b].parts[part-1].angle + RAND_BETWEEN(-2.0*M_PI/16, 2.0*M_PI/16);
      world->bots[b].parts[part].x = world->bots[b].parts[part-1].x - spacer*sin(world->bots[b].parts[part].angle);
      world->bots[b].parts[part].y = world->bots[b].parts[part-1].y - spacer*cos(world->bots[b].parts[part].angle);
      
      spacer *= 0.9;
    }
    NORMALISE_RAD(world->bots[b].parts[part].angle);
    
    world->bots[b].parts[part].num_eyes = 0;
    world->bots[b].parts[part].num_spikes = 0;
    world->bots[b].parts[part].num_ears = 0;
    world->bots[b].parts[part].radius = spacer;
    world->bots[b].parts[part].r = red - (float)part/world->bots[b].num_parts;
    world->bots[b].parts[part].g = green - (float)part/world->bots[b].num_parts;
    world->bots[b].parts[part].b = blue - (float)part/world->bots[b].num_parts;
  }
  
  float multiplier;
  
  // Eyes
  multiplier = RAND_BETWEEN(0.8, 1.0);
  bot_eye_add(&world->bots[b], 0, 1*(2.0*M_PI/12), 2.0*M_PI/5, 3.0*multiplier);
  bot_eye_add(&world->bots[b], 0, 11*(2.0*M_PI/12), 2.0*M_PI/5, 3.0*multiplier);
  bot_eye_add(&world->bots[b], world->bots[b].num_parts-1, M_PI, 2.0*M_PI/4, 1.5*multiplier);
  
  // Spikes (Caterpillar look)
  multiplier = RAND_BETWEEN(0.8, 1.0);
  bot_spike_add(&world->bots[b], 0, 0.8*multiplier, 0.0);
  bot_spike_add(&world->bots[b], 0, 0.6*multiplier, 5*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 0, 0.6*multiplier, 1*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 1, 0.6*multiplier, 5*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], 1, 0.6*multiplier, 1*(2.0*M_PI/6));
  for(i = 1; i < world->bots[b].num_parts-1; ++i)
  {
    bot_spike_add(&world->bots[b], i, 2.0*multiplier*world->bots[b].parts[i].radius, 4*(2.0*M_PI/6));
    bot_spike_add(&world->bots[b], i, 2.0*multiplier*world->bots[b].parts[i].radius, 2*(2.0*M_PI/6));
  }
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 3.0*multiplier*world->bots[b].parts[world->bots[b].num_parts-1].radius, 4*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 3.0*multiplier*world->bots[b].parts[world->bots[b].num_parts-1].radius, 2*(2.0*M_PI/6));
  bot_spike_add(&world->bots[b], world->bots[b].num_parts-1, 5.0*multiplier*world->bots[b].parts[world->bots[b].num_parts-1].radius, 2.0*M_PI/2);
  
  // Ears
  multiplier = RAND_BETWEEN(0.8, 1.0);
  bot_ear_add(&world->bots[b], 0, 5.0*multiplier);
  
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
  
  world->pellets[i].r = 0.0;
  world->pellets[i].g = RAND_BETWEEN(0.4, 0.5);
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
  printf("Generation: %i\n", world->generation);
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
  world->generation = 0;
  world->w = 96.0;
  world->h = 54.0;
  world->num_bots = 0;
  world->num_pellets = 0;
  world->grid_w = 16;
  world->grid_h = 9;
  
  srand(world->seed);
  world->grid = malloc(world->grid_w * sizeof(s_tile*));
  
  int w;
  for(w = 0; w < world->grid_w; ++w)
  {
    world->grid[w] = malloc(world->grid_h * sizeof(s_tile));
    int h;
    for(h = 0; h < world->grid_h; ++h)
    {
      // Green-Brown
      world->grid[w][h].food = 100;
      world->grid[w][h].r = RAND_BETWEEN(0.65, 0.75);
      world->grid[w][h].g = RAND_BETWEEN(0.55, 0.65);
      world->grid[w][h].b = RAND_BETWEEN(0.25, 0.35);
      // Green
      //world->grid[w][h].r = RAND_BETWEEN(0.2, 0.3);
      //world->grid[w][h].g = RAND_BETWEEN(0.4, 0.7);
      //world->grid[w][h].b = RAND_BETWEEN(0.1, 0.2);
    }
  }
  
  return 0;
}