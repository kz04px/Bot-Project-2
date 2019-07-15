#ifndef KEYBOARD_EVENT_HPP_INCLUDED
#define KEYBOARD_EVENT_HPP_INCLUDED

#include "event.hpp"

class KeyPressEvent : public Event {
   public:
    KeyPressEvent(int key, int scancode, int mods)
        : key_(key), scancode_(scancode), mods_(mods) {
    }
    EventType type() const override {
        return EventType::KeyPressEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::KeyboardGroup;
    }
    std::string string() const override {
        return "KeyPressEvent";
    }
    int key() const {
        return key_;
    }
    int scancode() const {
        return scancode_;
    }
    int mods() const {
        return mods_;
    }

   private:
    int key_;
    int scancode_;
    int mods_;
};

class KeyReleaseEvent : public Event {
   public:
    KeyReleaseEvent(int key, int scancode, int mods)
        : key_(key), scancode_(scancode), mods_(mods) {
    }
    EventType type() const override {
        return EventType::KeyReleaseEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::KeyboardGroup;
    }
    std::string string() const override {
        return "KeyReleaseEvent";
    }
    int key() const {
        return key_;
    }
    int scancode() const {
        return scancode_;
    }
    int mods() const {
        return mods_;
    }

   private:
    int key_;
    int scancode_;
    int mods_;
};

class KeyRepeatEvent : public Event {
   public:
    KeyRepeatEvent(int key, int scancode, int mods)
        : key_(key), scancode_(scancode), mods_(mods) {
    }
    EventType type() const override {
        return EventType::KeyRepeatEvent;
    }
    int groups() const override {
        return EventGroup::InputGroup | EventGroup::KeyboardGroup;
    }
    std::string string() const override {
        return "KeyRepeatEvent";
    }
    int key() const {
        return key_;
    }
    int scancode() const {
        return scancode_;
    }
    int mods() const {
        return mods_;
    }

   private:
    int key_;
    int scancode_;
    int mods_;
};

#endif
