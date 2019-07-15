#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <clog/clog.hpp>
#include <fstream>
#include "io.hpp"

std::string load_shader_file(const char* filename) {
    assert(filename);
    std::string result;
    std::string line;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            result += line + '\n';
        }
    }
    return result;
}

int create_shader(const char* filename, int type) {
    assert(filename);

    std::string shader_string = load_shader_file(filename);
    /*if (!shader_string) {
        clog::Log::get()->error("Failed to load ", filename);
        return -1;
    }*/

    int params = GL_TRUE;
    const char* source = shader_string.c_str();
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &source, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        clog::Log::get()->error("Failed to compile shader");
        return -2;
    }

    return s;
}
