#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include "../defs.hpp"
#include "../io.hpp"
#include "data.hpp"

bool camera_moving = false;

void glfw_window_size_callback(GLFWwindow *window, int width, int height) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    data.width_ = width;
    data.height_ = height;

    glViewport(0, 0, width, height);
}

void glfw_cursor_position_callback(GLFWwindow *window,
                                   double xpos,
                                   double ypos) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    static double xlast = 0;
    static double ylast = 0;

    if (camera_moving) {
        const float ratio = (float)data.width_ / data.height_;
        camera_x -= 40.0 * (xpos - xlast) / data.width_ * camera_zoom * ratio;
        camera_y += 40.0 * (ypos - ylast) / data.height_ * camera_zoom;
    }

    xlast = xpos;
    ylast = ypos;
}

void glfw_mouse_scroll_callback(GLFWwindow *window,
                                double xoffset,
                                double yoffset) {
    assert(window);
    if (yoffset > 0.0) {
        camera_zoom /= 1.1;
    } else {
        camera_zoom *= 1.1;
    }

    if (camera_zoom < 0.05) {
        camera_zoom = 0.05;
    } else if (camera_zoom > 20.0) {
        camera_zoom = 20.0;
    }
}

void glfw_mouse_button_callback(GLFWwindow *window,
                                int button,
                                int action,
                                int mods) {
    assert(window);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            camera_moving = (action == GLFW_PRESS);
            break;
        default:
            break;
    }
}

void glfw_keyboard_callback(GLFWwindow *window,
                            int key,
                            int scancode,
                            int action,
                            int mods) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    // Action:
    //  1 - Press
    //  2 - Repeat
    //  0 - Release

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, 1);
            break;
        case GLFW_KEY_SPACE:
            if (action == 1) {
                sim_data.paused = 1 - sim_data.paused;
            }
            break;
        case GLFW_KEY_1:
            if (action == 1) {
                sim_data.fps_max = 30;
            }
            break;
        case GLFW_KEY_2:
            if (action == 1) {
                sim_data.fps_max = 60;
            }
            break;
        case GLFW_KEY_3:
            if (action == 1) {
                sim_data.fps_max = 120;
            }
            break;
        case GLFW_KEY_4:
            if (action == 1) {
                sim_data.fps_max = -1;
            }
            break;
        case GLFW_KEY_F2:
            screenshot_tga("test.tga", data.width_, data.height_);
            break;
        default:
            std::cout << "Key: " << key << " " << action << std::endl;
            break;
    }
}
