#include <pthread.h>
#include "defs.h"
#include "matrix.h"

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
  glLineWidth(2.0);
  
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
  
  /*
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
  printf("Max physics FPS: %i\n", (int)(NUM_FRAMES/(t1-t0)));
  printf("\n");
  */
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
  
  s_buffers buffers_bots;
  buffers_init_bots(&buffers_bots);
  
  camera_x = world->w/2;
  camera_y = world->h/2;
  camera_zoom = 1.2;
  
  // Create simulation thread
  pthread_t sim_thread;
  sim_data.fps_max = 60;
  sim_data.fps = 0;
  sim_data.paused = 0;
  sim_data.quit = 0;
  sim_data.world = world;
  pthread_create(&sim_thread, NULL, simulate_world, &sim_data);
  
  // Find uniform
  GLint loc_vp_matrix = glGetUniformLocation(shader_program, "vp_matrix");
  if(loc_vp_matrix < 0) {print_log("ERROR: Could not find uniform vp_matrix\n"); return -1;}
  
  double current_seconds = 0.0;
  double last_seconds = glfwGetTime();
  
  // Drawing
  glClearColor(0.6, 0.6, 0.8, 1.0);
  while(!glfwWindowShouldClose(window))
  {
    // Set uniform
    s_mat4 vp_matrix = ortho(-20.0*window_ratio*camera_zoom + camera_x,20.0*window_ratio*camera_zoom + camera_x,
                             -20.0*camera_zoom + camera_y,20.0*camera_zoom + camera_y,
                             0.0,1.0);
    glUniformMatrix4fv(loc_vp_matrix, 1, GL_FALSE, vp_matrix.m);
    
    //update_fps_counter(window);
    
    // Update FPS display
    current_seconds = glfwGetTime();
    if(current_seconds - last_seconds >= 0.5)
    {
      char tmp[128];
      sprintf(tmp, "Bot Project 2 - %i fps - %f avg fitness", sim_data.fps, sim_data.world->average_fitness);
      glfwSetWindowTitle(window, tmp);
      last_seconds = current_seconds;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_program);
    
    // Background
    glBindVertexArray(buffers_background.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(0+0*sizeof(GL_UNSIGNED_INT)), world->grid_w * world->grid_h);
    
    // Pellets
    buffers_fill_pellets(world, &buffers_pellets);
    glBindVertexArray(buffers_pellets.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 8, GL_UNSIGNED_INT, (void*)(0+0*sizeof(GL_UNSIGNED_INT)), world->num_pellets);
    
    // Bots
    glBindVertexArray(buffers_bots.vao);
    int b;
    for(b = 0; b < world->num_bots; ++b)
    {
      if(world->bots[b].health <= 0) {continue;}
      
      int part;
      for(part = world->bots[b].num_parts-1; part >= 0; --part)
      {
        // Draw eye cones
        buffer_fill_bot_cones(&world->bots[b].parts[part], &buffers_bots);
        for(i = 0; i < world->bots[b].parts[part].num_eyes; ++i)
        {
          glDrawArrays(GL_LINES, 2 * (EYE_CONE_ACCURACY + 2)*i, 2*(EYE_CONE_ACCURACY + 2));
        }
        
        // Draw ear ranges
        buffer_fill_bot_ears(&world->bots[b].parts[part], &buffers_bots);
        glDrawArrays(GL_LINES, 0, 2*EAR_RANGE_ACCURACY*world->bots[b].parts[part].num_ears);
        
        // Draw spikes
        buffer_fill_bot_spikes(&world->bots[b].parts[part], &buffers_bots);
        glDrawArrays(GL_TRIANGLES, 0, 3*world->bots[b].parts[part].num_spikes);
        
        // Draw body
        float scale = 1.0;
        for(i = 0; i < part; ++i) {scale *= 0.9;}
        buffer_fill_bot_body(&world->bots[b].parts[part], &buffers_bots, scale);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
        
        // Draw eye borders
        buffer_fill_bot_eye_borders(&world->bots[b].parts[part], &buffers_bots);
        for(i = 0; i < world->bots[b].parts[part].num_eyes; ++i)
        {
          glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);
        }
        
        // Draw eyes
        buffer_fill_bot_eyes(&world->bots[b].parts[part], &buffers_bots);
        for(i = 0; i < world->bots[b].parts[part].num_eyes; ++i)
        {
          glDrawArrays(GL_TRIANGLE_FAN, 4*i, 4);
        }
      }
    }
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  sim_data.quit = 1;
  pthread_join(sim_thread, NULL);
  glfwTerminate();
  return 0;
}
