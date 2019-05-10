#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "simulation/world.hpp"
#include "window/window.hpp"

class Window;
class World;

class Application {
   public:
    Application();
    ~Application();
    void run();

   private:
    Window window_;
    World world_;
    bool quit_;
};

#endif
