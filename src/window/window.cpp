#include "window.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include "../log.hpp"
#include "callbacks.hpp"

Window::Window(const std::string &title, const int width, const int height) {
    data_.title_ = title;
    data_.width_ = width;
    data_.height_ = height;

    assert(width > 0);
    assert(height > 0);

    if (!glfwInit()) {
        Log::get()->error("glfwInit error ", glGetError());
        throw "asd";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window_) {
        Log::get()->error("glfwCreateWindow error ", glGetError());
        throw "weow";
    }

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, &data_);
    glfwSetWindowSizeCallback(window_, glfw_window_size_callback);
    glfwSetCursorPosCallback(window_, glfw_cursor_position_callback);
    glfwSetScrollCallback(window_, glfw_mouse_scroll_callback);
    glfwSetKeyCallback(window_, glfw_keyboard_callback);
    glfwSetMouseButtonCallback(window_, glfw_mouse_button_callback);

    glPointSize(3.0);
    glLineWidth(2.0);

    glewExperimental = GL_TRUE;
    glewInit();
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void Window::title(const std::string &title) {
    data_.title_ = title;
    glfwSetWindowTitle(window_, title.c_str());
}

int Window::width() {
    return data_.width_;
}

int Window::height() {
    return data_.height_;
}

void Window::clear() {
    glClearColor(0.529, 0.808, 0.922, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap_buffer() {
    glfwSwapBuffers(window_);
}

void Window::poll_events() {
    glfwPollEvents();
}

bool Window::should_close() {
    return glfwWindowShouldClose(window_);
}

void Window::set_callback(const std::function<void(Event &)> &func) {
    data_.callback_ = func;
}
