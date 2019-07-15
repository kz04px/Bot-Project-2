#ifndef WINDOW_EVENT_HPP_INCLUDED
#define WINDOW_EVENT_HPP_INCLUDED

#include "event.hpp"

class WindowResizeEvent : public Event {
   public:
    WindowResizeEvent(const int w, const int h) : width_(w), height_(h) {
    }
    EventType type() const override {
        return EventType::WindowResizeEvent;
    }
    int groups() const override {
        return EventGroup::WindowGroup;
    }
    std::string string() const override {
        return "WindowResizeEvent";
    }
    int width() const {
        return width_;
    }
    int height() const {
        return height_;
    }

   private:
    int width_;
    int height_;
};

class WindowCloseEvent : public Event {
   public:
    WindowCloseEvent() {
    }
    EventType type() const override {
        return EventType::WindowCloseEvent;
    }
    int groups() const override {
        return EventGroup::WindowGroup;
    }
    std::string string() const override {
        return "WindowCloseEvent";
    }

   private:
};

#endif
