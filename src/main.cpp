#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include "buffers.hpp"
#include "defs.hpp"
#include "io.hpp"
#include "log.hpp"
#include "shaders.hpp"
#include "simulation/simulation.hpp"
#include "window/window.hpp"

int main(int argc, char** argv) {
    bool benchmark = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-benchmark") {
            benchmark = true;
        }
    }

    Window window("Bot Project 2", 640, 480);

#ifndef NDEBUG
    const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);    // version as a string
    Log::get()->debug("Compiled: ", __TIME__, " ", __DATE__);
    Log::get()->debug("OpenGL Renderer: ", renderer);
    Log::get()->debug("OpenGL Version: ", version);
#endif

    // Create shaders
    GLuint vs = create_shader("shaders//vertex_shader.glsl", GL_VERTEX_SHADER);
    if (vs == 0) {
        Log::get()->error("GL_VERTEX_SHADER creation error");
        return 1;
    }
    GLuint fs =
        create_shader("shaders//fragment_shader.glsl", GL_FRAGMENT_SHADER);
    if (fs == 0) {
        Log::get()->error("GL_FRAGMENT_SHADER creation error");
        return 1;
    }

    // Create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    World world;
    world_init(&world);
    world_bots_add(&world, 20);
    world_pellets_add(&world, 150);

#ifndef NDEBUG
    world_print_details(&world);
#endif

    if (benchmark) {
        const int NUM_FRAMES = 10000;
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

    // Does the GPU support current FBO configuration?
    GLenum err = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch (err) {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            Log::get()->info("Framebuffer status: complete");
            break;
        default:
            Log::get()->error("Framebuffer status");
            break;
    }

    Buffers buffers_background;
    buffers_init_background(&buffers_background);
    buffers_fill_background(&world, &buffers_background);

    Buffers bufferPellets;
    buffers_init_pellets(&bufferPellets);

    Buffers bufferBots;
    buffers_init_bots(&bufferBots);

    camera_x = world.w / 2;
    camera_y = world.h / 2;
    camera_zoom = 1.2;

    // Create simulation thread
    sim_data.fps_max = 60;
    sim_data.fps = 0;
    sim_data.paused = 0;
    sim_data.quit = 0;
    std::thread sim_thread(simulate_world, std::ref(world));

    // Find uniform
    GLint loc_vp_matrix = glGetUniformLocation(shader_program, "vp_matrix");
    if (loc_vp_matrix < 0) {
        Log::get()->error("Could not find uniform vp_matrix");
        return -1;
    }

    double current_seconds = 0.0;
    double last_seconds = glfwGetTime();

    // Drawing
    glClearColor(0.6, 0.6, 0.8, 1.0);
    while (!window.should_close()) {
        const float ratio = (float)window.width() / window.height();
        // Set uniform
        glm::mat4 vp_matrix = glm::ortho(-20.0 * ratio * camera_zoom + camera_x,
                                         20.0 * ratio * camera_zoom + camera_x,
                                         -20.0 * camera_zoom + camera_y,
                                         20.0 * camera_zoom + camera_y,
                                         0.0,
                                         1.0);
        glUniformMatrix4fv(
            loc_vp_matrix, 1, GL_FALSE, glm::value_ptr(vp_matrix));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);

        // Background
        glBindVertexArray(buffers_background.vao);
        glDrawElementsInstanced(GL_TRIANGLES,
                                12,
                                GL_UNSIGNED_INT,
                                (void*)(0 + 0 * sizeof(GL_UNSIGNED_INT)),
                                world.grid_w * world.grid_h);

        // Pellets
        buffers_fill_pellets(&world, &bufferPellets);
        glBindVertexArray(bufferPellets.vao);
        glDrawElementsInstanced(GL_TRIANGLES,
                                8,
                                GL_UNSIGNED_INT,
                                (void*)(0 + 0 * sizeof(GL_UNSIGNED_INT)),
                                world.num_pellets);

        // Bots
        glBindVertexArray(bufferBots.vao);
        for (int b = 0; b < world.num_bots; ++b) {
            if (world.bots[b].health <= 0) {
                continue;
            }

            for (int part = world.bots[b].num_parts - 1; part >= 0; --part) {
                // Draw eye cones
                buffer_fill_bot_cones(&world.bots[b].parts[part], &bufferBots);
                for (int i = 0; i < world.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_LINES,
                                 2 * (EYE_CONE_ACCURACY + 2) * i,
                                 2 * (EYE_CONE_ACCURACY + 2));
                }

                // Draw ear ranges
                buffer_fill_bot_ears(&world.bots[b].parts[part], &bufferBots);
                glDrawArrays(GL_LINES,
                             0,
                             2 * EAR_RANGE_ACCURACY *
                                 world.bots[b].parts[part].num_ears);

                // Draw spikes
                buffer_fill_bot_spikes(&world.bots[b].parts[part], &bufferBots);
                glDrawArrays(
                    GL_TRIANGLES, 0, 3 * world.bots[b].parts[part].num_spikes);

                // Draw body
                float scale = 1.0;
                for (int i = 0; i < part; ++i) {
                    scale *= 0.9;
                }
                buffer_fill_bot_body(
                    &world.bots[b].parts[part], &bufferBots, scale);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

                // Draw eye borders
                buffer_fill_bot_eye_borders(&world.bots[b].parts[part],
                                            &bufferBots);
                for (int i = 0; i < world.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_TRIANGLE_FAN, 4 * i, 4);
                }

                // Draw eyes
                buffer_fill_bot_eyes(&world.bots[b].parts[part], &bufferBots);
                for (int i = 0; i < world.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_TRIANGLE_FAN, 4 * i, 4);
                }
            }
        }

        window.poll_events();
        window.swap_buffer();
    }

    sim_data.quit = 1;
    if (sim_thread.joinable()) {
        sim_thread.join();
    }
    return 0;
}
