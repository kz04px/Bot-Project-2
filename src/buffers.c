#include "defs.h"

int buffers_fill_bots(s_world* world)
{
  assert(world != NULL);
  
  return 0;
}

int buffers_init_bots(s_buffers* buffers)
{
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

int buffers_init_pellets(s_buffers* buffers)
{
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

int buffers_init_background(s_buffers* buffers)
{
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
  
  // Vertices
  background_points[0] = r_x*sin(0*angle + hangle); // p1
  background_points[1] = r_y*cos(0*angle + hangle);
  background_points[2] = r_x*sin(1*angle + hangle); // p2
  background_points[3] = r_y*cos(1*angle + hangle);
  background_points[4] = r_x*sin(2*angle + hangle); // p3
  background_points[5] = r_y*cos(2*angle + hangle);
  background_points[6] = r_x*sin(3*angle + hangle); // p4
  background_points[7] = r_y*cos(3*angle + hangle);
  background_points[8] = r_x*sin(4*angle + hangle); // p5
  background_points[9] = r_y*cos(4*angle + hangle);
  background_points[10] = r_x*sin(5*angle + hangle); // p6
  background_points[11] = r_y*cos(5*angle + hangle);
  glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof *background_points, background_points, GL_STATIC_DRAW);
  
  // Indices
  GLuint indexData[] = {
    0,1,2,
    0,2,3,
    0,3,4,
    0,4,5
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*12, indexData, GL_STATIC_DRAW);
  
  // Positions
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
  glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*world->grid_w*world->grid_h, translationData, GL_STATIC_DRAW);
  
  // Colours
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
  glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*world->grid_w*world->grid_h, colourData, GL_STATIC_DRAW);
  
  return 0;
}