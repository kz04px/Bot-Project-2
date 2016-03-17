#include "defs.h"
#include "matrix.h"

// 144 ns
// 133 ns
// 100 ns
//  90 ns
//  
//  
// ...
//  36 ns

void update_fps_counter(GLFWwindow* window)
{
  static double previous_seconds = 0;
  static int frame_count = 0;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if(elapsed_seconds > 0.25)
  {
    previous_seconds = current_seconds;
    double fps = (double)frame_count/elapsed_seconds;
    char tmp[128];
    sprintf(tmp, "Bot Project 2 - %.2ffps", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }
  frame_count++;
}

int main()
{
  int i;
  int r;
  GLenum err;
  
  #ifndef NDEBUG
  print_log("--- Log Start ---\n");
  print_log("Date: %s\n", __DATE__);
  print_log("Time: %s\n", __TIME__);
  #endif
  
  r = load_settings("settings.dat");
  if(r != 0)
  {
    // Default values
    window_width = 640;
    window_height = 480;
    window_fullscreen = GL_FALSE;
    print_log("ERROR: Failed to load settings.dat\n");
  }
  window_ratio = (float)window_width/window_height;
  
  // start GL context and O/S window using the GLFW helper library
  if(!glfwInit())
  {
    print_log("ERROR: could not start GLFW3\n");
    return 1;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);
  
  GLFWwindow *window = NULL;
  if(window_fullscreen == GL_TRUE)
  {
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
  }
  else
  {
    window = glfwCreateWindow(window_width, window_height, "Bot Project 2", NULL, NULL);
  }
  if(!window)
  {
    print_log("ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, glfw_window_size_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
  glfwSetScrollCallback(window, glfw_mouse_scroll_callback);
  glfwSetKeyCallback(window, glfw_keyboard_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();
  while((err = glGetError()) != GL_NO_ERROR)
  {
    print_log("ERROR: glewInit() (%i)\n", err);
  }
  
  #ifndef NDEBUG
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  
  printf("Debug info:\n");
  printf(" Date: %s\n", __DATE__);
  printf(" Time: %s\n", __TIME__);
  printf(" Renderer: %s\n", renderer);
  printf(" OpenGL version supported %s\n", version);
  printf("\n");
  #endif
  
  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);
	//glEnable(GL_TEXTURE_2D);
  glPointSize(3.0);
  
  // Create shaders
  GLuint vs = create_shader("shaders//vertex_shader.glsl", GL_VERTEX_SHADER);
  if(vs == 0) {print_log("ERROR: create_shader (GL_VERTEX_SHADER) %i\n", vs);}
  GLuint fs = create_shader("shaders//fragment_shader.glsl", GL_FRAGMENT_SHADER);
  if(fs == 0) {print_log("ERROR: create_shader (GL_FRAGMENT_SHADER) %i\n", vs);}
  
  // Create shader program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);
  
  s_world *world = malloc(1*sizeof(s_world));
  world_init(world);
  world_pellets_add(world, 150);
  world_bots_add(world, 20);
  
  #ifndef NDEBUG
  world_print_details(world);
  
  #define NUM_FRAMES 10000
  double t0 = glfwGetTime();
  for(i = 0; i < NUM_FRAMES; ++i)
  {
    world_simulate_frame(world);
  }
  double t1 = glfwGetTime();
  
  printf("Simulation Benchmark:\n");
  printf("Frames: %i\n", NUM_FRAMES);
  printf("Total time: %.2gs\n", t1-t0);
  printf("Time per frame: %.4gns\n", (t1-t0)/NUM_FRAMES*1000.0*1000.0);
  printf("\n");
  #endif
  
  // Does the GPU support current FBO configuration?
  err = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  switch(err)
  {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      print_log("Framebuffer status: complete\n");
      break;
    default:
      print_log("ERROR: Framebuffer status: %i\n", err);
      break;
  }
  
  s_buffers buffers_background;
  buffers_init_background(&buffers_background);
  buffers_fill_background(world, &buffers_background);
  
  s_buffers buffers_pellets;
  buffers_init_pellets(&buffers_pellets);
  
  camera_x = world->w/2;
  camera_y = world->h/2;
  camera_zoom = 1.2;
  
  // Find uniform
  GLint loc_vp_matrix = glGetUniformLocation(shader_program, "vp_matrix");
  if(loc_vp_matrix < 0) {print_log("ERROR: Could not find uniform vp_matrix\n"); return -1;}
  
  GLuint vao_test, vbo_test, tbo_test, cbo_test, rbo_test;
  
  glGenVertexArrays(1, &vao_test);
  glBindVertexArray(vao_test);
  
  glGenBuffers(1, &vbo_test);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  glGenBuffers(1, &tbo_test);
  glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  glGenBuffers(1, &cbo_test);
  glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  glGenBuffers(1, &rbo_test);
  glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (char*)0 + 0*sizeof(GLfloat));
  
  float angle = 2.0*M_PI/5;
  float size = 0.25;
  float bot_vertices[2 * 5 * MAX_PARTS];
  for(i = 0; i < MAX_PARTS; ++i)
  {
    // p1
    bot_vertices[2*5*i + 0] = size*sin(0*angle);
    bot_vertices[2*5*i + 1] = size*cos(0*angle);
    // p2
    bot_vertices[2*5*i + 2] = size*sin(1*angle);
    bot_vertices[2*5*i + 3] = size*cos(1*angle);
    // p3
    bot_vertices[2*5*i + 4] = size*sin(2*angle);
    bot_vertices[2*5*i + 5] = size*cos(2*angle);
    // p4
    bot_vertices[2*5*i + 6] = size*sin(3*angle);
    bot_vertices[2*5*i + 7] = size*cos(3*angle);
    // p5
    bot_vertices[2*5*i + 8] = size*sin(4*angle);
    bot_vertices[2*5*i + 9] = size*cos(4*angle);
    
    size *= 0.9;
  }
  
  // Drawing
  glClearColor(0.6, 0.6, 0.8, 1.0);
  while(!glfwWindowShouldClose(window))
  {
    // Simulate
    world_simulate_frame(world);
    
    // Fill buffers
    buffers_fill_pellets(world, &buffers_pellets);
    
    
    // Set uniform
    s_mat4 vp_matrix = ortho(-20.0*window_ratio*camera_zoom + camera_x,20.0*window_ratio*camera_zoom + camera_x,
                             -20.0*camera_zoom + camera_y,20.0*camera_zoom + camera_y,
                             0.0,1.0);
    glUniformMatrix4fv(loc_vp_matrix, 1, GL_FALSE, vp_matrix.m);
    
    update_fps_counter(window);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    
    // Background
    glBindVertexArray(buffers_background.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(0+0*sizeof(GL_UNSIGNED_INT)), world->grid_w * world->grid_h);
    
    // Pellets
    glBindVertexArray(buffers_pellets.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 8, GL_UNSIGNED_INT, (void*)(0+0*sizeof(GL_UNSIGNED_INT)), world->num_pellets);
    
    // Bots
    glBindVertexArray(vao_test);
    int b;
    for(b = 0; b < world->num_bots; ++b)
    {
      int s;
      int e;
      
      /*** Eyes - Cones ***/
      float eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes];
      float eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes];
      float eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes];
      float eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes];
      for(e = 0; e < world->bots[b].num_eyes; ++e)
      {
        int part = world->bots[b].eyes[e].part;
        
        // p0
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 0] = 0.0;
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 1] = 0.0;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 0] = world->bots[b].parts[part].x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 1] = world->bots[b].parts[part].y;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 0] = world->bots[b].eyes[e].r;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 1] = world->bots[b].eyes[e].g;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 2] = world->bots[b].eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(0) + 0] = 0.0;
        // p1
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 2] = 0.0;
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 3] = world->bots[b].eyes[e].dist;;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 2] = world->bots[b].parts[part].x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(0) + 3] = world->bots[b].parts[part].y;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 3] = world->bots[b].eyes[e].r;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 4] = world->bots[b].eyes[e].g;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(0) + 5] = world->bots[b].eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(0) + 1] = world->bots[b].parts[part].angle +
                                                                        world->bots[b].eyes[e].angle -
                                                                        world->bots[b].eyes[e].fov/2;
        // p3
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 0] = 0.0;
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 1] = 0.0;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 0] = world->bots[b].parts[part].x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 1] = world->bots[b].parts[part].y;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 0] = world->bots[b].eyes[e].r;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 1] = world->bots[b].eyes[e].g;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 2] = world->bots[b].eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(1) + 0] = 0.0;
        // p4
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 2] = 0.0;
        eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 3] = world->bots[b].eyes[e].dist;;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 2] = world->bots[b].parts[part].x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(1) + 3] = world->bots[b].parts[part].y;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 3] = world->bots[b].eyes[e].r;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 4] = world->bots[b].eyes[e].g;
        eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(1) + 5] = world->bots[b].eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(1) + 1] = world->bots[b].parts[part].angle +
                                                                        world->bots[b].eyes[e].angle +
                                                                        world->bots[b].eyes[e].fov/2;
        
        int n;
        for(n = 0; n < EYE_CONE_ACCURACY; ++n)
        {
          // pn
          eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 0] = 0.0;
          eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 1] = world->bots[b].eyes[e].dist;
          eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 0] = world->bots[b].parts[part].x;
          eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 1] = world->bots[b].parts[part].y;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 0] = world->bots[b].eyes[e].r;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 1] = world->bots[b].eyes[e].g;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 2] = world->bots[b].eyes[e].b;
          eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(n+2) + 0] = world->bots[b].parts[part].angle +
                                                                            world->bots[b].eyes[e].angle -
                                                                            world->bots[b].eyes[e].fov/2 +
                                                                            (float)n*(world->bots[b].eyes[e].fov/EYE_CONE_ACCURACY);
          // pn+1
          eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 2] = 0.0;
          eye_cone_vertices[ 4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 3] = world->bots[b].eyes[e].dist;
          eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 2] = world->bots[b].parts[part].x;
          eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2)*e + 4*(n+2) + 3] = world->bots[b].parts[part].y;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 3] = world->bots[b].eyes[e].r;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 4] = world->bots[b].eyes[e].g;
          eye_cone_colours[  6 * (EYE_CONE_ACCURACY + 2)*e + 6*(n+2) + 5] = world->bots[b].eyes[e].b;
          eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2)*e + 2*(n+2) + 1] = world->bots[b].parts[part].angle +
                                                                            world->bots[b].eyes[e].angle -
                                                                            world->bots[b].eyes[e].fov/2 +
                                                                            (float)(n+1)*(world->bots[b].eyes[e].fov/EYE_CONE_ACCURACY);
        }
      }
      
      // Vertices
      glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
      glBufferData(GL_ARRAY_BUFFER, 4 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes * sizeof *eye_cone_vertices, eye_cone_vertices, GL_STATIC_DRAW);
      
      // Positions
      glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
      glBufferData(GL_ARRAY_BUFFER, 4 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_parts * sizeof *eye_cone_positions, eye_cone_positions, GL_STATIC_DRAW);
      
      // Colours
      glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
      glBufferData(GL_ARRAY_BUFFER, 6 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes * sizeof *eye_cone_colours, eye_cone_colours, GL_STATIC_DRAW);
      
      // Rotations
      glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * (EYE_CONE_ACCURACY + 2) * world->bots[b].num_eyes * sizeof *eye_cone_rotations, eye_cone_rotations, GL_STATIC_DRAW);
      
      // Draw eye cones
      for(i = 0; i < world->bots[b].num_eyes; ++i)
      {
        glDrawArrays(GL_LINES, 2 * (EYE_CONE_ACCURACY + 2)*i, 2*(EYE_CONE_ACCURACY + 2));
      }
      /*** Eyes - Cones ***/
      
      
      
      /*** Spikes ***/
      float spike_vertices[2 * 3 * world->bots[b].num_spikes];
      float spike_positions[2 * 3 * world->bots[b].num_spikes];
      float spike_colours[3 * 3 * world->bots[b].num_spikes];
      float spike_rotations[1 * 3 * world->bots[b].num_spikes];
      for(s = 0; s < world->bots[b].num_spikes; ++s)
      {
        int part = world->bots[b].spikes[s].part;
        
        // p0
        spike_vertices[2*3*s + 0] =  0.0;
        spike_vertices[2*3*s + 1] =  world->bots[b].spikes[s].length;
        spike_positions[2*3*s + 0] = world->bots[b].parts[part].x;
        spike_positions[2*3*s + 1] = world->bots[b].parts[part].y;
        spike_colours[3*3*s +  0] =  world->bots[b].spikes[s].r;
        spike_colours[3*3*s +  1] =  world->bots[b].spikes[s].g;
        spike_colours[3*3*s +  2] =  world->bots[b].spikes[s].b;
        spike_rotations[1*3*s + 0] = world->bots[b].parts[part].angle + world->bots[b].spikes[s].angle;
        
        // p1
        spike_vertices[2*3*s + 2] =  0.025;
        spike_vertices[2*3*s + 3] =  0.5*world->bots[b].parts[part].radius;
        spike_positions[2*3*s + 2] = world->bots[b].parts[part].x;
        spike_positions[2*3*s + 3] = world->bots[b].parts[part].y;
        spike_colours[3*3*s +  3] =  world->bots[b].spikes[s].r;
        spike_colours[3*3*s +  4] =  world->bots[b].spikes[s].g;
        spike_colours[3*3*s +  5] =  world->bots[b].spikes[s].b;
        spike_rotations[1*3*s + 1] = world->bots[b].parts[part].angle + world->bots[b].spikes[s].angle;
        
        // p2
        spike_vertices[2*3*s + 4] = -0.025;
        spike_vertices[2*3*s + 5] =  0.5*world->bots[b].parts[part].radius;
        spike_positions[2*3*s + 4] = world->bots[b].parts[part].x;
        spike_positions[2*3*s + 5] = world->bots[b].parts[part].y;
        spike_colours[3*3*s +  6] =  world->bots[b].spikes[s].r;
        spike_colours[3*3*s +  7] =  world->bots[b].spikes[s].g;
        spike_colours[3*3*s +  8] =  world->bots[b].spikes[s].b;
        spike_rotations[1*3*s + 2] = world->bots[b].parts[part].angle + world->bots[b].spikes[s].angle;
      }
      
      // Vertices
      glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 3 * world->bots[b].num_spikes * sizeof *spike_vertices, spike_vertices, GL_STATIC_DRAW);
      
      // Positions
      glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 3 * world->bots[b].num_spikes * sizeof *spike_positions, spike_positions, GL_STATIC_DRAW);
      
      // Colours
      glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
      glBufferData(GL_ARRAY_BUFFER, 3 * 3 * world->bots[b].num_spikes * sizeof *spike_colours, spike_colours, GL_STATIC_DRAW);
      
      // Rotations
      glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
      glBufferData(GL_ARRAY_BUFFER, 1 * 3 * world->bots[b].num_spikes * sizeof *spike_rotations, spike_rotations, GL_STATIC_DRAW);
      
      // Draw spikes
      glDrawArrays(GL_TRIANGLES, 0, 3*world->bots[b].num_spikes);
      /*** Spikes ***/
      
      
      /*** Body parts ***/
      // Vertices
      glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 5 * world->bots[b].num_parts * sizeof *bot_vertices, bot_vertices, GL_STATIC_DRAW);
      
      // Positions
      float bot_positions[2 * 5 * MAX_PARTS];
      for(i = 0; i < world->bots[b].num_parts; ++i)
      {
        // p1
        bot_positions[2*5*i + 0] = world->bots[b].parts[i].x;
        bot_positions[2*5*i + 1] = world->bots[b].parts[i].y;
        // p2
        bot_positions[2*5*i + 2] = world->bots[b].parts[i].x;
        bot_positions[2*5*i + 3] = world->bots[b].parts[i].y;
        // p3
        bot_positions[2*5*i + 4] = world->bots[b].parts[i].x;
        bot_positions[2*5*i + 5] = world->bots[b].parts[i].y;
        // p4
        bot_positions[2*5*i + 6] = world->bots[b].parts[i].x;
        bot_positions[2*5*i + 7] = world->bots[b].parts[i].y;
        // p5
        bot_positions[2*5*i + 8] = world->bots[b].parts[i].x;
        bot_positions[2*5*i + 9] = world->bots[b].parts[i].y;
      }
      glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 5 * world->bots[b].num_parts * sizeof *bot_positions, bot_positions, GL_STATIC_DRAW);
      
      // Colours
      float bot_colours[3 * 5 * MAX_PARTS];
      for(i = 0; i < world->bots[b].num_parts; ++i)
      {
        // p1
        bot_colours[3*5*i +  0] = world->bots[b].parts[i].r;
        bot_colours[3*5*i +  1] = world->bots[b].parts[i].g;
        bot_colours[3*5*i +  2] = world->bots[b].parts[i].b;
        // p2
        bot_colours[3*5*i +  3] = world->bots[b].parts[i].r;
        bot_colours[3*5*i +  4] = world->bots[b].parts[i].g;
        bot_colours[3*5*i +  5] = world->bots[b].parts[i].b;
        // p3
        bot_colours[3*5*i +  6] = world->bots[b].parts[i].r;
        bot_colours[3*5*i +  7] = world->bots[b].parts[i].g;
        bot_colours[3*5*i +  8] = world->bots[b].parts[i].b;
        // p4
        bot_colours[3*5*i +  9] = world->bots[b].parts[i].r;
        bot_colours[3*5*i + 10] = world->bots[b].parts[i].g;
        bot_colours[3*5*i + 11] = world->bots[b].parts[i].b;
        // p5
        bot_colours[3*5*i + 12] = world->bots[b].parts[i].r;
        bot_colours[3*5*i + 13] = world->bots[b].parts[i].g;
        bot_colours[3*5*i + 14] = world->bots[b].parts[i].b;
      }
      glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
      glBufferData(GL_ARRAY_BUFFER, 3 * 5 * world->bots[b].num_parts * sizeof *bot_colours, bot_colours, GL_STATIC_DRAW);
      
      // Rotations
      float bot_rotations[3 * 5 * MAX_PARTS];
      for(i = 0; i < world->bots[b].num_parts; ++i)
      {
        // p1
        bot_rotations[5*i + 0] = world->bots[b].parts[i].angle;
        // p2
        bot_rotations[5*i + 1] = world->bots[b].parts[i].angle;
        // p3
        bot_rotations[5*i + 2] = world->bots[b].parts[i].angle;
        // p4
        bot_rotations[5*i + 3] = world->bots[b].parts[i].angle;
        // p5
        bot_rotations[5*i + 4] = world->bots[b].parts[i].angle;
      }
      glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
      glBufferData(GL_ARRAY_BUFFER, 5 * world->bots[b].num_parts * sizeof *bot_rotations, bot_rotations, GL_STATIC_DRAW);
      
      // Draw body parts
      //for(i = 0; i < world->bots[b].num_parts; ++i)
      for(i = world->bots[b].num_parts-1; i >= 0; --i)
      {
        glDrawArrays(GL_TRIANGLE_FAN, 5*i, 5);
      }
      /*** Body parts ***/
      
      
      float eye_size = 0.02;
      float eye_vertices[2 * 4 * world->bots[b].num_eyes];
      float eye_positions[2 * 4 * world->bots[b].num_eyes];
      float eye_colours[3 * 4 * world->bots[b].num_eyes];
      float eye_rotations[1 * 4 * world->bots[b].num_eyes];
      /*** Eyes - Borders ***/
      for(e = 0; e < world->bots[b].num_eyes; ++e)
      {
        int part = world->bots[b].eyes[e].part;
        
        // p0
        eye_vertices[2*4*e + 0] = -1.2*eye_size;
        eye_vertices[2*4*e + 1] = -1.2*eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 0] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 1] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  0] =  0.0;
        eye_colours[3*4*e +  1] =  0.0;
        eye_colours[3*4*e +  2] =  0.0;
        eye_rotations[1*4*e + 0] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p1
        eye_vertices[2*4*e + 2] = -1.2*eye_size;
        eye_vertices[2*4*e + 3] =  1.2*eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 2] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 3] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  3] =  0.0;
        eye_colours[3*4*e +  4] =  0.0;
        eye_colours[3*4*e +  5] =  0.0;
        eye_rotations[1*4*e + 1] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p2
        eye_vertices[2*4*e + 4] =  1.2*eye_size;
        eye_vertices[2*4*e + 5] =  1.2*eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 4] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 5] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  6] =  0.0;
        eye_colours[3*4*e +  7] =  0.0;
        eye_colours[3*4*e +  8] =  0.0;
        eye_rotations[1*4*e + 2] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p3
        eye_vertices[2*4*e + 6] =  1.2*eye_size;
        eye_vertices[2*4*e + 7] = -1.2*eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 6] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 7] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  9] =  0.0;
        eye_colours[3*4*e + 10] =  0.0;
        eye_colours[3*4*e + 11] =  0.0;
        eye_rotations[1*4*e + 3] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
      }
      
      // Vertices
      glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 4 * world->bots[b].num_eyes * sizeof *eye_vertices, eye_vertices, GL_STATIC_DRAW);
      
      // Positions
      glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 4 * world->bots[b].num_parts * sizeof *eye_positions, eye_positions, GL_STATIC_DRAW);
      
      // Colours
      glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
      glBufferData(GL_ARRAY_BUFFER, 3 * 4 * world->bots[b].num_eyes * sizeof *eye_colours, eye_colours, GL_STATIC_DRAW);
      
      // Rotations
      glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
      glBufferData(GL_ARRAY_BUFFER, 1 * 4 * world->bots[b].num_eyes * sizeof *eye_rotations, eye_rotations, GL_STATIC_DRAW);
      
      // Draw eyes
      for(i = 0; i < world->bots[b].num_eyes; ++i)
      {
        glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);
      }
      /*** Eyes - Borders ***/
      
      
      /*** Eyes ***/
      for(e = 0; e < world->bots[b].num_eyes; ++e)
      {
        int part = world->bots[b].eyes[e].part;
        
        // p0
        eye_vertices[2*4*e + 0] = -eye_size;
        eye_vertices[2*4*e + 1] = -eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 0] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 1] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  0] =  world->bots[b].eyes[e].r;
        eye_colours[3*4*e +  1] =  world->bots[b].eyes[e].g;
        eye_colours[3*4*e +  2] =  world->bots[b].eyes[e].b;
        eye_rotations[1*4*e + 0] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p1
        eye_vertices[2*4*e + 2] = -eye_size;
        eye_vertices[2*4*e + 3] =  eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 2] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 3] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  3] =  world->bots[b].eyes[e].r;
        eye_colours[3*4*e +  4] =  world->bots[b].eyes[e].g;
        eye_colours[3*4*e +  5] =  world->bots[b].eyes[e].b;
        eye_rotations[1*4*e + 1] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p2
        eye_vertices[2*4*e + 4] =  eye_size;
        eye_vertices[2*4*e + 5] =  eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 4] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 5] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  6] =  world->bots[b].eyes[e].r;
        eye_colours[3*4*e +  7] =  world->bots[b].eyes[e].g;
        eye_colours[3*4*e +  8] =  world->bots[b].eyes[e].b;
        eye_rotations[1*4*e + 2] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
        
        // p3
        eye_vertices[2*4*e + 6] =  eye_size;
        eye_vertices[2*4*e + 7] = -eye_size + 0.6*world->bots[b].parts[part].radius;
        eye_positions[2*4*e + 6] = world->bots[b].parts[part].x;
        eye_positions[2*4*e + 7] = world->bots[b].parts[part].y;
        eye_colours[3*4*e +  9] =  world->bots[b].eyes[e].r;
        eye_colours[3*4*e + 10] =  world->bots[b].eyes[e].g;
        eye_colours[3*4*e + 11] =  world->bots[b].eyes[e].b;
        eye_rotations[1*4*e + 3] = world->bots[b].parts[part].angle + world->bots[b].eyes[e].angle;
      }
      
      // Vertices
      glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 4 * world->bots[b].num_eyes * sizeof *eye_vertices, eye_vertices, GL_STATIC_DRAW);
      
      // Positions
      glBindBuffer(GL_ARRAY_BUFFER, tbo_test);
      glBufferData(GL_ARRAY_BUFFER, 2 * 4 * world->bots[b].num_parts * sizeof *eye_positions, eye_positions, GL_STATIC_DRAW);
      
      // Colours
      glBindBuffer(GL_ARRAY_BUFFER, cbo_test);
      glBufferData(GL_ARRAY_BUFFER, 3 * 4 * world->bots[b].num_eyes * sizeof *eye_colours, eye_colours, GL_STATIC_DRAW);
      
      // Rotations
      glBindBuffer(GL_ARRAY_BUFFER, rbo_test);
      glBufferData(GL_ARRAY_BUFFER, 1 * 4 * world->bots[b].num_eyes * sizeof *eye_rotations, eye_rotations, GL_STATIC_DRAW);
      
      // Draw eyes
      for(i = 0; i < world->bots[b].num_eyes; ++i)
      {
        glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);
      }
      /*** Eyes ***/
    }
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
  return 0;
}
