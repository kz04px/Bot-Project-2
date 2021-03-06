#include "application.hpp"
#include <clog/clog.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "buffers.hpp"
#include "events/event.hpp"
#include "events/keyboard-event.hpp"
#include "events/mouse-event.hpp"
#include "events/window-event.hpp"
#include "io.hpp"
#include "shaders.hpp"
#include "simulation/world.hpp"
#include "window/window.hpp"

Application::Application()
    : window_{"Bot Project 2", 640, 480},
      world_{},
      paused_{false},
      quit_{false},
      camera_moving_{false} {
    clog::Log::get()->info("Application constructor");
    window_.set_callback(
        std::bind(&Application::on_event, this, std::placeholders::_1));

    // Create shaders
    GLuint vs = create_shader("shaders//vertex_shader.glsl", GL_VERTEX_SHADER);
    if (vs == 0) {
        clog::Log::get()->error("GL_VERTEX_SHADER creation error");
        return;
    }
    GLuint fs =
        create_shader("shaders//fragment_shader.glsl", GL_FRAGMENT_SHADER);
    if (fs == 0) {
        clog::Log::get()->error("GL_FRAGMENT_SHADER creation error");
        return;
    }

    // Create shader program
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);
}

Application::~Application() {
    clog::Log::get()->info("Application destructor");
}

void Application::on_event(Event &e) {
    switch (e.type()) {
        case EventType::MouseDownEvent:
            on_mouse_down(static_cast<MouseDownEvent &>(e));
            break;
        case EventType::MouseUpEvent:
            on_mouse_up(static_cast<MouseUpEvent &>(e));
            break;
        case EventType::MouseMoveEvent:
            on_mouse_move(static_cast<MouseMoveEvent &>(e));
            break;
        case EventType::MouseScrollEvent:
            on_mouse_scroll(static_cast<MouseScrollEvent &>(e));
            break;
        case EventType::KeyPressEvent:
            on_key_press(static_cast<KeyPressEvent &>(e));
            break;
        case EventType::WindowResizeEvent:
            on_window_resize(static_cast<WindowResizeEvent &>(e));
            break;
        default:
            break;
    }
}

void Application::on_mouse_down(MouseDownEvent &e) {
    camera_moving_ = true;
}

void Application::on_mouse_up(MouseUpEvent &e) {
    camera_moving_ = false;
}

void Application::on_mouse_move(MouseMoveEvent &e) {
    static double xlast = 0;
    static double ylast = 0;

    if (camera_moving_) {
        const float ratio = window_.width() / window_.height();
        const float dx = (e.x() - xlast) / window_.width();
        const float dy = (e.y() - ylast) / window_.height();
        camera_.shift(dx, dy);
    }

    xlast = e.x();
    ylast = e.y();
}

void Application::on_mouse_scroll(MouseScrollEvent &e) {
    if (e.up()) {
        camera_.zoom_out();
    } else {
        camera_.zoom_in();
    }
}

void Application::on_key_press(KeyPressEvent &e) {
    switch (e.key()) {
        case GLFW_KEY_ESCAPE:
            quit_ = true;
            break;
        case GLFW_KEY_SPACE:
            paused_ = !paused_;
            std::cout << "Paused: " << paused_ << std::endl;
            break;
        case GLFW_KEY_1:
            // fps_max = 30;
            break;
        case GLFW_KEY_2:
            // fps_max = 60;
            break;
        case GLFW_KEY_3:
            // fps_max = 120;
            break;
        case GLFW_KEY_4:
            // fps_max = -1;
            break;
        case GLFW_KEY_F2:
            // screenshot_tga(
            //    "test.tga", window_.data_.width_, window_.data_.height_);
            break;
        default:
            std::cout << "Key: " << e.key() << std::endl;
            break;
    }
}

void Application::on_window_resize(WindowResizeEvent &e) {
    glViewport(0, 0, e.width(), e.height());
    camera_.set_ratio(e.width(), e.height());
}

void Application::run() {
    clog::Log::get()->info("Application run");

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

    // Find uniform
    GLint loc_vp_matrix = glGetUniformLocation(shader_program_, "vp_matrix");
    if (loc_vp_matrix < 0) {
        clog::Log::get()->error("Could not find uniform vp_matrix");
        return;
    }

    while (!quit_ && !window_.should_close()) {
        // Events
        window_.poll_events();

        // Simulate frame
        if (!paused_) {
            world_simulate_frame(&world_);
        }

        // Render frame
        const float ratio = (float)window_.width() / window_.height();
        // Set uniform
        glm::mat4 vp_matrix = camera_.matrix();

        glUniformMatrix4fv(
            loc_vp_matrix, 1, GL_FALSE, glm::value_ptr(vp_matrix));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program_);

        // Background
        glBindVertexArray(buffers_background.vao);
        glDrawElementsInstanced(GL_TRIANGLES,
                                12,
                                GL_UNSIGNED_INT,
                                (void *)(0 + 0 * sizeof(GL_UNSIGNED_INT)),
                                world_.grid_w * world_.grid_h);

        // Pellets
        buffers_fill_pellets(&world_, &bufferPellets);
        glBindVertexArray(bufferPellets.vao);
        glDrawElementsInstanced(GL_TRIANGLES,
                                8,
                                GL_UNSIGNED_INT,
                                (void *)(0 + 0 * sizeof(GL_UNSIGNED_INT)),
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

        window_.swap_buffer();
    }
}
