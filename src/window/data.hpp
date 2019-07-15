#ifndef WINDOW_DATA_HPP
#define WINDOW_DATA_HPP

#include <functional>
#include <string>

class Event;

struct WindowData {
    std::string title_ = "untitled";
    int width_ = 480;
    int height_ = 480;
    bool fullscreen_ = false;
    bool vsync_ = false;
    double mouse_x_ = 0.0;
    double mouse_y_ = 0.0;
    std::function<void(Event &)> callback_;
};

#endif
