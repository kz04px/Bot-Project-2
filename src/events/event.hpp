#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <functional>
#include <string>

enum class EventType
{
    NoneEvent = 0,
    MouseDownEvent,
    MouseUpEvent,
    MouseMoveEvent,
    KeyPressEvent,
    KeyRepeatEvent,
    KeyReleaseEvent,
    WindowMoveEvent,
    WindowResizeEvent,
    WindowCloseEvent,
    WindowMinimiseEvent
};

enum EventGroup
{
    NoneGroup = 0,
    EngineGroup = 1,
    InputGroup = 2,
    MouseGroup = 4,
    KeyboardGroup = 8,
    WindowGroup = 16
};

class Event {
   public:
    virtual EventType type() const = 0;
    virtual int groups() const = 0;
    virtual std::string string() const = 0;
    bool handled_ = false;

   private:
};

inline std::ostream &operator<<(std::ostream &os, const Event &e) {
    os << e.string();
    return os;
}

class EventManager {
   public:
    EventManager(Event &event) : event_(event) {
    }

    template <typename T>
    bool dispatch(std::function<bool(T &)> func) {
        if (event_.type() == T::type()) {
            event_.handled_ = func(*(T *)&event_);
            return true;
        }
        return false;
    }

   private:
    Event &event_;
};

#endif
