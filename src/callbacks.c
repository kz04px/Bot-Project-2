#include "defs.h"

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
  window_width = width;
  window_height = height;
  window_ratio = (float)window_width/window_height;

  glViewport(0, 0, window_width, window_height);
}

int camera_moving = 0;

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  static double xlast = 0;
  static double ylast = 0;
  
  if(camera_moving == 1)
  {
    camera_x -= 40.0*(xpos - xlast)/window_width*camera_zoom*window_ratio;
    camera_y += 40.0*(ypos - ylast)/window_height*camera_zoom;
  }
  
  xlast = xpos;
  ylast = ypos;
}

void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(yoffset > 0.0) {camera_zoom /= 1.1;}
  else              {camera_zoom *= 1.1;}
  
       if(camera_zoom < 0.05) {camera_zoom = 0.05;}
  else if(camera_zoom > 20.0) {camera_zoom = 20.0;}
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  // Button:
  // 0 - Left
  // 1 - Right
  // 2 - Middle

  // Action:
  // 1 - Down
  // 0 - Up
  
  if(button == 1)
  {
    camera_moving = action;
  }
}

void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  // Action:
  //  1 - Press
  //  2 - Repeat
  //  0 - Release

  switch(key)
  {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, 1);
      break;
    case GLFW_KEY_F2:
      screenshot_tga("test.tga", window_width, window_height);
      break;
    default:
      printf("Key: %c(%i) %i\n", key, key, action);
      break;
  }
}
