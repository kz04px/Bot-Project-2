#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "simulation/world.hpp"
#include "window/window.hpp"

class Window;
class World;
class Event;
class KeyPressEvent;
class MouseDownEvent;
class MouseUpEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class WindowResizeEvent;

class Application {
   public:
    Application();
    ~Application();
    void on_event(Event &e);
    void on_mouse_down(MouseDownEvent &e);
    void on_mouse_up(MouseUpEvent &e);
    void on_mouse_move(MouseMoveEvent &e);
    void on_mouse_scroll(MouseScrollEvent &e);
    void on_key_press(KeyPressEvent &e);
    void on_window_resize(WindowResizeEvent &e);
    void run();

   private:
    Window window_;
    World world_;
    GLuint shader_program_;
    Camera camera_;
    bool paused_;
    bool quit_;
    bool camera_moving_;
};

#endif
