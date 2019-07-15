#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "../events/keyboard-event.hpp"
#include "../events/mouse-event.hpp"
#include "../events/window-event.hpp"
#include "data.hpp"

void glfw_window_size_callback(GLFWwindow *window, int width, int height) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    data.width_ = width;
    data.height_ = height;

    WindowResizeEvent event(width, height);
    data.callback_(event);
}

void glfw_cursor_position_callback(GLFWwindow *window,
                                   double xpos,
                                   double ypos) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseMoveEvent event(xpos, ypos);
    data.callback_(event);
}

void glfw_mouse_scroll_callback(GLFWwindow *window,
                                double xoffset,
                                double yoffset) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseScrollEvent event(yoffset > 0.0);
    data.callback_(event);
}

void glfw_mouse_button_callback(GLFWwindow *window,
                                int button,
                                int action,
                                int mods) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        MouseDownEvent event(data.mouse_x_, data.mouse_y_, button);
        data.callback_(event);
    } else if (action == GLFW_RELEASE) {
        MouseUpEvent event(data.mouse_x_, data.mouse_y_, button);
        data.callback_(event);
    } else {
        assert(false);
    }
}

void glfw_keyboard_callback(GLFWwindow *window,
                            int key,
                            int scancode,
                            int action,
                            int mods) {
    assert(window);
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS) {
        KeyPressEvent event(key, scancode, mods);
        data.callback_(event);
    } else if (action == GLFW_RELEASE) {
        KeyReleaseEvent event(key, scancode, mods);
        data.callback_(event);
    } else if (action == GLFW_REPEAT) {
        KeyRepeatEvent event(key, scancode, mods);
        data.callback_(event);
    } else {
        assert(false);
    }
}
