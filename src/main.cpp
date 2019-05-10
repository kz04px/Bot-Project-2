#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include "application.hpp"
#include "log.hpp"
#include "simulation/world.hpp"

int main(int argc, char** argv) {
    bool benchmark = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-benchmark") {
            benchmark = true;
        }
    }

    if (benchmark) {
        const int NUM_FRAMES = 10000;

        World world;
        world_init(&world);
        world_bots_add(&world, 20);
        world_pellets_add(&world, 150);

        const auto t0 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < NUM_FRAMES; ++i) {
            world_simulate_frame(&world);
        }
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto diff =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        std::cout << "Simulation Benchmark:" << std::endl;
        std::cout << "Frames: " << NUM_FRAMES << std::endl;
        std::cout << "Total time: " << diff.count() << "ms" << std::endl;
        std::cout << "Time per frame: " << (float)diff.count() / NUM_FRAMES
                  << "ms" << std::endl;
        std::cout << "Max physics FPS: "
                  << 1000 * (float)NUM_FRAMES / diff.count() << std::endl;

        Log::get()->info("Benchmark: ", diff.count(), "ms");
    }

    try {
        Application app;
        app.run();
    } catch (...) {
        Log::get()->error("Application exception");
        return 1;
    }

    return 0;
}
