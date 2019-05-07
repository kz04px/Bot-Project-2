#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void glfw_cursor_position_callback(GLFWwindow *window,
                                   double xpos,
                                   double ypos);
void glfw_mouse_scroll_callback(GLFWwindow *window,
                                double xoffset,
                                double yoffset);
void glfw_keyboard_callback(GLFWwindow *window,
                            int key,
                            int scancode,
                            int action,
                            int mods);
void glfw_mouse_button_callback(GLFWwindow *window,
                                int button,
                                int action,
                                int mods);

#endif
