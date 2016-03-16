#include "defs.h"

int buffers_fill_bots(s_world* world)
{
  assert(world != NULL);
  
  /*
  int b;
  float bot_points[10];
  
  // vao and vbo handles
  GLuint vao, vbo, tbo, ibo, cbo, rbo;
  
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float angle = 2.0*M_PI/5.0;
  float size = 0.25;
  
  // p1
  bot_points[0] = size*sin(0*angle);
  bot_points[1] = size*cos(0*angle);
  // p2
  bot_points[2] = size*sin(1*angle);
  bot_points[3] = size*cos(1*angle);
  // p3
  bot_points[4] = size*sin(2*angle);
  bot_points[5] = size*cos(2*angle);
  // p4
  bot_points[6] = size*sin(3*angle);
  bot_points[7] = size*cos(3*angle);
  // p5
  bot_points[8] = size*sin(4*angle);
  bot_points[9] = size*cos(4*angle);
  
  glBufferData(GL_ARRAY_BUFFER, 10 * sizeof *bot_points, bot_points, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  // generate and bind the index buffer object
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  
  GLuint indexData[] = {
    0,1,2,
    0,2,3,
    0,3,4
  };
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*10, indexData, GL_STATIC_DRAW);
  
  // Positions
  glGenBuffers(1, &tbo);
  glBindBuffer(GL_ARRAY_BUFFER, tbo);
  
  GLfloat translationData[2 * world->grid_w * world->grid_h];
  for(b = 0; b < world->num_bots; ++b)
  {
    translationData[2*b + 0] = world->bots[b].x;
    translationData[2*b + 1] = world->bots[b].y;
  }
 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*world->num_bots, translationData, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(1, 1);
  
  // Colours
  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  
  GLfloat colourData[3 * world->num_bots];
  for(b = 0; b < world->num_bots; ++b)
  {
    colourData[3*b + 0] = world->bots[b].r;
    colourData[3*b + 1] = world->bots[b].g;
    colourData[3*b + 2] = world->bots[b].b;
  }
 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*world->num_bots, colourData, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(2, 1);
  
  // Rotations
  glGenBuffers(1, &rbo);
  glBindBuffer(GL_ARRAY_BUFFER, rbo);
  
  GLfloat rotationData[world->num_bots];
  for(b = 0; b < world->num_bots; ++b)
  {
    rotationData[b] = world->bots[b].angle;
  }
 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*1*world->num_bots, rotationData, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(3, 1);
  
  return vao;
  */
  return 0;
}

int buffers_init_bots(s_world* world, s_buffers* buffers)
{
  assert(world != NULL);
  assert(buffers != NULL);
  
  glGenVertexArrays(1, &buffers->vao);
  glBindVertexArray(buffers->vao);
  
  // Indices
  glGenBuffers(1, &buffers->ibo);
  
  // Vertices
  glGenBuffers(1, &buffers->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  // Positions
  glGenBuffers(1, &buffers->tbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(1, 1);
  
  // Colours
  glGenBuffers(1, &buffers->cbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(2, 1);
  
  // Rotation
  glGenBuffers(1, &buffers->rbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(3, 1);
  
  return 0;
}

int buffers_init_pellets(s_world* world, s_buffers* buffers)
{
  assert(world != NULL);
  assert(buffers != NULL);
  
  glGenVertexArrays(1, &buffers->vao);
  glBindVertexArray(buffers->vao);
  
  // Indices
  glGenBuffers(1, &buffers->ibo);
  
  // Vertices
  glGenBuffers(1, &buffers->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  // Positions
  glGenBuffers(1, &buffers->tbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(1, 1);
  
  // Colours
  glGenBuffers(1, &buffers->cbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(2, 1);
  
  return 0;
}

int buffers_fill_pellets(s_world* world, s_buffers* buffers)
{
  assert(world != NULL);
  assert(buffers != NULL);
  
  int p;
  float pellet_points[8];
  
  glBindVertexArray(buffers->vao);
  
  // Vertices
  float size = 0.1;
  pellet_points[0] = -size; // p1
  pellet_points[1] = -size;
  pellet_points[2] = -size; // p2
  pellet_points[3] =  size;
  pellet_points[4] =  size; // p3
  pellet_points[5] =  size;
  pellet_points[6] =  size; // p4
  pellet_points[7] = -size;
  glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
  glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof *pellet_points, pellet_points, GL_STATIC_DRAW);
  
  // Indices
  GLuint indexData[] = {
    0,1,2,
    0,2,3
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*8, indexData, GL_STATIC_DRAW);
  
  // Positions
  GLfloat translationData[2 * world->grid_w * world->grid_h];
  for(p = 0; p < world->num_pellets; ++p)
  {
    translationData[2*p + 0] = world->pellets[p].x;
    translationData[2*p + 1] = world->pellets[p].y;
  }
  glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*world->num_pellets, translationData, GL_STATIC_DRAW);
  
  // Colours
  GLfloat colourData[3 * world->num_pellets];
  for(p = 0; p < world->num_pellets; ++p)
  {
    colourData[3*p + 0] = world->pellets[p].r;
    colourData[3*p + 1] = world->pellets[p].g;
    colourData[3*p + 2] = world->pellets[p].b;
  }
  glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*world->grid_w*world->grid_h, colourData, GL_STATIC_DRAW);
  
  return 0;
}

int buffers_fill_background(s_world* world, s_buffers* buffers)
{
  assert(world != NULL);
  assert(buffers != NULL);
  
  int i;
  int w;
  int h;
  float background_points[12];
  
  float angle = 2.0*M_PI/6.0;
  float hangle = angle/2.0;
  float r_y = 0.5*(world->h / world->grid_h)/0.866;
  float r_x = (world->w / world->grid_w) - r_y*sin(hangle);
  
  glGenVertexArrays(1, &buffers->vao);
  glBindVertexArray(buffers->vao);
  
  glGenBuffers(1, &buffers->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
  
  // p1
  background_points[0] = r_x*sin(0*angle + hangle);
  background_points[1] = r_y*cos(0*angle + hangle);
  // p2
  background_points[2] = r_x*sin(1*angle + hangle);
  background_points[3] = r_y*cos(1*angle + hangle);
  // p3
  background_points[4] = r_x*sin(2*angle + hangle);
  background_points[5] = r_y*cos(2*angle + hangle);
  // p4
  background_points[6] = r_x*sin(3*angle + hangle);
  background_points[7] = r_y*cos(3*angle + hangle);
  // p5
  background_points[8] = r_x*sin(4*angle + hangle);
  background_points[9] = r_y*cos(4*angle + hangle);
  // p6
  background_points[10] = r_x*sin(5*angle + hangle);
  background_points[11] = r_y*cos(5*angle + hangle);
  
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof *background_points, background_points, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  // generate and bind the index buffer object
  glGenBuffers(1, &buffers->ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ibo);
  
  GLuint indexData[] = {
    0,1,2,
    0,2,3,
    0,3,4,
    0,4,5
  };
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*12, indexData, GL_STATIC_DRAW);
  
  // Positions
  glGenBuffers(1, &buffers->tbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
  
  i = 0;
  GLfloat translationData[2 * world->grid_w * world->grid_h];
  for(w = 0; w < world->grid_w; ++w)
  {
    for(h = 0; h < world->grid_h; ++h)
    {
      translationData[2*i + 0] = (float)w/world->grid_w * world->w;
      translationData[2*i + 1] = (float)h/world->grid_h * world->h + 0.5*(w%2)*(world->h/world->grid_h);
      i++;
    }
  }
 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*world->grid_w*world->grid_h, translationData, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(1, 1);
  
  // Colours
  glGenBuffers(1, &buffers->cbo);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
  
  i = 0;
  GLfloat colourData[3 * world->grid_w * world->grid_h];
  for(w = 0; w < world->grid_w; ++w)
  {
    for(h = 0; h < world->grid_h; ++h)
    {
      colourData[3*i + 0] = world->grid[w][h].r;
      colourData[3*i + 1] = world->grid[w][h].g;
      colourData[3*i + 2] = world->grid[w][h].b;
      i++;
    }
  }
 
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*world->grid_w*world->grid_h, colourData, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  glVertexAttribDivisor(2, 1);
  
  return 0;
}