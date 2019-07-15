#ifndef MOUSE_EVENT_HPP_INCLUDED
#define MOUSE_EVENT_HPP_INCLUDED

#include "event.hpp"

class MouseDownEvent : public Event {
   public:
    MouseDownEvent(const int x, const int y, const int button)
        : x_(x), y_(y), button_(button) {
    }
    EventType type() const override {
        return EventType::MouseDownEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::MouseGroup;
    }
    std::string string() const override {
        return "MouseDownEvent";
    }
    int x() const {
        return x_;
    }
    int y() const {
        return y_;
    }
    int button() const {
        return button_;
    }

   private:
    int x_;
    int y_;
    int button_;
};

class MouseUpEvent : public Event {
   public:
    MouseUpEvent(const int x, const int y, const int button)
        : x_(x), y_(y), button_(button) {
    }
    EventType type() const override {
        return EventType::MouseUpEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::MouseGroup;
    }
    std::string string() const override {
        return "MouseUpEvent";
    }
    int x() const {
        return x_;
    }
    int y() const {
        return y_;
    }
    int button() const {
        return button_;
    }

   private:
    int x_;
    int y_;
    int button_;
};

class MouseMoveEvent : public Event {
   public:
    MouseMoveEvent(const int x, const int y) : x_(x), y_(y) {
    }
    EventType type() const override {
        return EventType::MouseMoveEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::MouseGroup;
    }
    std::string string() const override {
        return "MouseMoveEvent";
    }
    int x() const {
        return x_;
    }
    int y() const {
        return y_;
    }

   private:
    int x_;
    int y_;
};

class MouseScrollEvent : public Event {
   public:
    MouseScrollEvent(const bool up) : up_(up) {
    }
    EventType type() const override {
        return EventType::MouseScrollEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::MouseGroup;
    }
    std::string string() const override {
        return "MouseScrollEvent";
    }
    bool up() const {
        return up_;
    }
    bool down() const {
        return !up_;
    }

   private:
    bool up_;
};

#endif
