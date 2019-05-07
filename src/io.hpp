#ifndef IO_HPP
#define IO_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int load_settings(const char *filename);
int print_log(const char *format, ...);
void print_log_shader_info(GLuint shader_index);
int screenshot_tga(char *path, int w, int h);

#endif
