#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include "data.hpp"
#include <functional>

struct GLFWwindow;
class Event;

class Window {
   public:
    Window(const std::string &title, const int width, const int height);
    ~Window();
    void title(const std::string &title);
    int width();
    int height();
    void clear();
    void swap_buffer();
    void poll_events();
    bool should_close();
    void set_callback(const std::function<void(Event &)> &func);

   private:
    WindowData data_;
    GLFWwindow *window_;
};

#endif
