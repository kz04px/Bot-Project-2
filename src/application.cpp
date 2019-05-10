#include "application.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include "buffers.hpp"
#include "defs.hpp"
#include "io.hpp"
#include "log.hpp"
#include "shaders.hpp"
#include "simulation/simulation.hpp"
#include "simulation/world.hpp"
#include "window/window.hpp"

Application::Application() : window_{"Bot Project 2", 640, 480}, world_{} {
    Log::get()->info("Application constructor");
}

Application::~Application() {
    Log::get()->info("Application destructor");
}

void Application::run() {
    Log::get()->info("Application run");

    // Create shaders
    GLuint vs = create_shader("shaders//vertex_shader.glsl", GL_VERTEX_SHADER);
    if (vs == 0) {
        Log::get()->error("GL_VERTEX_SHADER creation error");
        return;
    }
    GLuint fs =
        create_shader("shaders//fragment_shader.glsl", GL_FRAGMENT_SHADER);
    if (fs == 0) {
        Log::get()->error("GL_FRAGMENT_SHADER creation error");
        return;
    }

    // Create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    world_init(&world_);
    world_bots_add(&world_, 20);
    world_pellets_add(&world_, 150);

#ifndef NDEBUG
    world_print_details(&world_);
#endif

    Buffers buffers_background;
    buffers_init_background(&buffers_background);
    buffers_fill_background(&world_, &buffers_background);

    Buffers bufferPellets;
    buffers_init_pellets(&bufferPellets);

    Buffers bufferBots;
    buffers_init_bots(&bufferBots);

    camera_x = world_.w / 2;
    camera_y = world_.h / 2;
    camera_zoom = 1.2;

    // Create simulation thread
    sim_data.fps_max = 60;
    sim_data.fps = 0;
    sim_data.paused = 0;
    sim_data.quit = 0;
    std::thread sim_thread(simulate_world, std::ref(world_));

    // Find uniform
    GLint loc_vp_matrix = glGetUniformLocation(shader_program, "vp_matrix");
    if (loc_vp_matrix < 0) {
        Log::get()->error("Could not find uniform vp_matrix");
        return;
    }

    while (!window_.should_close()) {
        const float ratio = (float)window_.width() / window_.height();
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
                                world_.grid_w * world_.grid_h);

        // Pellets
        buffers_fill_pellets(&world_, &bufferPellets);
        glBindVertexArray(bufferPellets.vao);
        glDrawElementsInstanced(GL_TRIANGLES,
                                8,
                                GL_UNSIGNED_INT,
                                (void*)(0 + 0 * sizeof(GL_UNSIGNED_INT)),
                                world_.num_pellets);

        // Bots
        glBindVertexArray(bufferBots.vao);
        for (int b = 0; b < world_.num_bots; ++b) {
            if (world_.bots[b].health <= 0) {
                continue;
            }

            for (int part = world_.bots[b].num_parts - 1; part >= 0; --part) {
                // Draw eye cones
                buffer_fill_bot_cones(&world_.bots[b].parts[part], &bufferBots);
                for (int i = 0; i < world_.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_LINES,
                                 2 * (EYE_CONE_ACCURACY + 2) * i,
                                 2 * (EYE_CONE_ACCURACY + 2));
                }

                // Draw ear ranges
                buffer_fill_bot_ears(&world_.bots[b].parts[part], &bufferBots);
                glDrawArrays(GL_LINES,
                             0,
                             2 * EAR_RANGE_ACCURACY *
                                 world_.bots[b].parts[part].num_ears);

                // Draw spikes
                buffer_fill_bot_spikes(&world_.bots[b].parts[part],
                                       &bufferBots);
                glDrawArrays(
                    GL_TRIANGLES, 0, 3 * world_.bots[b].parts[part].num_spikes);

                // Draw body
                float scale = 1.0;
                for (int i = 0; i < part; ++i) {
                    scale *= 0.9;
                }
                buffer_fill_bot_body(
                    &world_.bots[b].parts[part], &bufferBots, scale);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

                // Draw eye borders
                buffer_fill_bot_eye_borders(&world_.bots[b].parts[part],
                                            &bufferBots);
                for (int i = 0; i < world_.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_TRIANGLE_FAN, 4 * i, 4);
                }

                // Draw eyes
                buffer_fill_bot_eyes(&world_.bots[b].parts[part], &bufferBots);
                for (int i = 0; i < world_.bots[b].parts[part].num_eyes; ++i) {
                    glDrawArrays(GL_TRIANGLE_FAN, 4 * i, 4);
                }
            }
        }

        window_.poll_events();
        window_.swap_buffer();
    }

    sim_data.quit = 1;
    if (sim_thread.joinable()) {
        sim_thread.join();
    }
}
