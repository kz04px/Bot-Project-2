#include "world.hpp"
#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>

#define DEG_TO_RAD(x) (x * 0.0174532925)
#define RAD_TO_DEG(x) (x / 0.0174532925)
#define RAND_BETWEEN(a, b) ((b - a) * ((float)rand() / RAND_MAX) + a)
#define NORMALISE_RAD(x) \
    (x >= 2.0 * M_PI) ? x = x - 2.0 * M_PI : ((x < 0) ? x = x + 2.0 * M_PI : x)

const float wiggle[32] = {
    0.000000,  0.195090,  0.382683,  0.555570,  0.707107,  0.831470,  0.923880,
    0.980785,  1.000000,  0.980785,  0.923880,  0.831470,  0.707107,  0.555570,
    0.382683,  0.195090,  0.000000,  -0.195090, -0.382683, -0.555570, -0.707107,
    -0.831470, -0.923880, -0.980785, -1.000000, -0.980785, -0.923880, -0.831470,
    -0.707107, -0.555570, -0.382683, 0.195090};

float angle_difference(float x, float y) {
    float dif = x - y;
    NORMALISE_RAD(dif);
    if (dif > M_PI) {
        return (2.0 * M_PI - dif);
    } else {
        return dif;
    }
}

int world_simulate_frame(World *world) {
    assert(world);

    int num_bots_alive = 0;

    if (world->generation % 10 == 0 && world->frame == 0) {
        std::cout << "Generation: " << world->generation << std::endl;
    }

    for (int b = 0; b < world->num_bots; ++b) {
        if (world->bots[b].health <= 0) {
            continue;
        }
        num_bots_alive++;

        // Reset inputs
        for (int part = 0; part < world->bots[b].num_parts; ++part) {
            // Spikes
            for (int s = 0; s < world->bots[b].parts[part].num_spikes; ++s) {
                world->bots[b].parts[part].spikes[s].r -=
                    0.005 * (world->bots[b].parts[part].spikes[s].r - 0.8);
                world->bots[b].parts[part].spikes[s].g -=
                    0.005 * (world->bots[b].parts[part].spikes[s].g - 0.8);
                world->bots[b].parts[part].spikes[s].b -=
                    0.005 * (world->bots[b].parts[part].spikes[s].b - 0.8);
            }

            // Ears
            for (int ear = 0; ear < world->bots[b].parts[part].num_ears;
                 ++ear) {
                world->bots[b].parts[part].ears[ear].str = 0.0;
            }

            // Eyes
            for (int e = 0; e < world->bots[b].parts[part].num_eyes; ++e) {
                world->bots[b].parts[part].eyes[e].str = 0.0;
                world->bots[b].parts[part].eyes[e].r = 0.0;
                world->bots[b].parts[part].eyes[e].g = 0.0;
                world->bots[b].parts[part].eyes[e].b = 0.0;
            }
        }

        // Eating
        for (int p = 0; p < world->num_pellets; ++p) {
            float dx = world->bots[b].parts[0].x - world->pellets[p].x;
            if (fabs(dx) > world->bots[b].parts[0].radius) {
                continue;
            }

            float dy = world->bots[b].parts[0].y - world->pellets[p].y;
            if (fabs(dy) > world->bots[b].parts[0].radius) {
                continue;
            }

            float dist = sqrt(dx * dx + dy * dy);

            if (dist <= world->bots[b].parts[0].radius) {
                world->bots[b].energy += world->pellets[p].energy;

                if (world->pellets[p].energy > 0) {
                    // fnn_weights_reinforce(&world->bots[b].fnn);
                    world->pellets_eaten_good++;
                } else {
                    // fnn_weights_decay(&world->bots[b].fnn);
                    world->pellets_eaten_bad++;
                }

                world_pellet_replace(world, p);
                p--;
                continue;
            }
        }

        for (int part = 0; part < world->bots[b].num_parts; ++part) {
            // Skip the body part has no eyes or ears
            if (world->bots[b].parts[part].num_eyes == 0 &&
                world->bots[b].parts[part].num_ears == 0 &&
                world->bots[b].parts[part].num_spikes == 0) {
                continue;
            }

            // Pellets
            for (int p = 0; p < world->num_pellets; ++p) {
                float dx = world->bots[b].parts[part].x - world->pellets[p].x;
                if (fabs(dx) > MAX_VIEW_DIST) {
                    continue;
                }

                float dy = world->bots[b].parts[part].y - world->pellets[p].y;
                if (fabs(dy) > MAX_VIEW_DIST) {
                    continue;
                }

                float dist = sqrt(dx * dx + dy * dy);
                if (dist > MAX_VIEW_DIST) {
                    continue;
                }

                float angle = atan2(dx, dy) + M_PI;

                // Eyes
                for (int e = 0; e < world->bots[b].parts[part].num_eyes; ++e) {
                    float absolute_angle =
                        world->bots[b].parts[part].angle +
                        world->bots[b].parts[part].eyes[e].angle;
                    NORMALISE_RAD(absolute_angle);

                    float angle_dif = angle_difference(angle, absolute_angle);
                    NORMALISE_RAD(angle_dif);
                    NORMALISE_RAD(angle_dif);
                    NORMALISE_RAD(angle_dif);

                    if (angle_dif >
                        world->bots[b].parts[part].eyes[e].fov / 2) {
                        continue;
                    }
                    if (1.0 - dist / world->bots[b].parts[part].eyes[e].dist <
                        world->bots[b].parts[part].eyes[e].str) {
                        continue;
                    }

                    world->bots[b].parts[part].eyes[e].str =
                        1.0 - dist / world->bots[b].parts[part].eyes[e].dist;
                    world->bots[b].parts[part].eyes[e].r = world->pellets[p].r;
                    world->bots[b].parts[part].eyes[e].g = world->pellets[p].g;
                    world->bots[b].parts[part].eyes[e].b = world->pellets[p].b;
                }
            }

            // Bots
            for (int b2 = 0; b2 < world->num_bots; ++b2) {
                if (b == b2 || world->bots[b2].health <= 0) {
                    continue;
                }

                float dx_test =
                    world->bots[b].parts[0].x - world->bots[b2].parts[0].x;
                if (fabs(dx_test) > 2 * MAX_VIEW_DIST) {
                    continue;
                }

                float dy_test =
                    world->bots[b].parts[0].y - world->bots[b2].parts[0].y;
                if (fabs(dy_test) > 2 * MAX_VIEW_DIST) {
                    continue;
                }

                for (int part2 = 0; part2 < world->bots[b2].num_parts;
                     ++part2) {
                    float dx = world->bots[b].parts[part].x -
                               world->bots[b2].parts[part2].x;
                    float dy = world->bots[b].parts[part].y -
                               world->bots[b2].parts[part2].y;
                    float dist = sqrt(dx * dx + dy * dy);
                    float angle = atan2(dx, dy) + M_PI;

                    // Spikes
                    for (int s = 0; s < world->bots[b].parts[part].num_spikes;
                         ++s) {
                        if (dist > world->bots[b].parts[part].spikes[s].length +
                                       world->bots[b2].parts[part2].radius) {
                            continue;
                        }

                        if (dist > world->bots[b2].parts[part2].radius) {
                            float absolute_angle =
                                world->bots[b].parts[part].angle +
                                world->bots[b].parts[part].spikes[s].angle;
                            NORMALISE_RAD(absolute_angle);

                            float angle_dif =
                                angle_difference(angle, absolute_angle);
                            NORMALISE_RAD(angle_dif);
                            NORMALISE_RAD(angle_dif);
                            NORMALISE_RAD(angle_dif);

                            if (angle_dif > 2.0 * M_PI / 32) {
                                continue;
                            }
                        }

                        world->bots[b2].health--;
                        world->bots[b].energy++;
                        world->bots[b].parts[part].spikes[s].r = 1.0;
                        world->bots[b].parts[part].spikes[s].g = 0.0;
                        world->bots[b].parts[part].spikes[s].b = 0.0;
                    }

                    // Ears
                    for (int ear = 0; ear < world->bots[b].parts[part].num_ears;
                         ++ear) {
                        if (dist > world->bots[b].parts[part].ears[ear].dist) {
                            continue;
                        }

                        if (1.0 -
                                dist /
                                    world->bots[b].parts[part].ears[ear].dist <
                            world->bots[b].parts[part].ears[ear].str) {
                            continue;
                        }

                        world->bots[b].parts[part].ears[ear].str =
                            1.0 -
                            dist / world->bots[b].parts[part].ears[ear].dist;
                    }

                    // Eyes
                    for (int e = 0; e < world->bots[b].parts[part].num_eyes;
                         ++e) {
                        if (dist > world->bots[b].parts[part].eyes[e].dist) {
                            continue;
                        }

                        float absolute_angle =
                            world->bots[b].parts[part].angle +
                            world->bots[b].parts[part].eyes[e].angle;
                        NORMALISE_RAD(absolute_angle);

                        float angle_dif =
                            angle_difference(angle, absolute_angle);
                        NORMALISE_RAD(angle_dif);
                        NORMALISE_RAD(angle_dif);
                        NORMALISE_RAD(angle_dif);

                        if (angle_dif >
                            world->bots[b].parts[part].eyes[e].fov / 2) {
                            continue;
                        }
                        if (1.0 -
                                dist / world->bots[b].parts[part].eyes[e].dist <
                            world->bots[b].parts[part].eyes[e].str) {
                            continue;
                        }

                        world->bots[b].parts[part].eyes[e].str =
                            1.0 -
                            dist / world->bots[b].parts[part].eyes[e].dist;
                        world->bots[b].parts[part].eyes[e].r =
                            world->bots[b2].parts[0].r;
                        world->bots[b].parts[part].eyes[e].g =
                            world->bots[b2].parts[0].g;
                        world->bots[b].parts[part].eyes[e].b =
                            world->bots[b2].parts[0].b;
                    }
                }
            }
        }

        // Set brain inputs
        int eye_offset = 1;
        int ear_offset = eye_offset + 4 * world->bots[b].total_eyes;
        int pos = 0;

        world->bots[b].fnn.neurons[0][0].in = RAND_BETWEEN(0.0, 1.0);

        // Eyes
        for (int part = 0; part < world->bots[b].num_parts; ++part) {
            for (int e = 0; e < world->bots[b].parts[part].num_eyes; ++e) {
                world->bots[b].fnn.neurons[0][4 * pos + 0 + eye_offset].in =
                    world->bots[b].parts[part].eyes[e].str;
                world->bots[b].fnn.neurons[0][4 * pos + 1 + eye_offset].in =
                    world->bots[b].parts[part].eyes[e].r;
                world->bots[b].fnn.neurons[0][4 * pos + 2 + eye_offset].in =
                    world->bots[b].parts[part].eyes[e].g;
                world->bots[b].fnn.neurons[0][4 * pos + 3 + eye_offset].in =
                    world->bots[b].parts[part].eyes[e].b;
                pos++;
            }
        }

        // Ears
        for (int part = 0; part < world->bots[b].num_parts; ++part) {
            for (int e = 0; e < world->bots[b].parts[part].num_ears; ++e) {
                world->bots[b].fnn.neurons[0][4 * pos + 0 + ear_offset].in =
                    world->bots[b].parts[part].ears[e].str;
                pos++;
            }
        }

        // Random movement
        float output_0 = 1.0;
        float output_1 = RAND_BETWEEN(0.0, 1.0);

        // Basic movement
        /*
        output_0 = 1.0;
             if(world->bots[b].parts[0].eyes[0].str >
        world->bots[b].parts[0].eyes[1].str) {output_1 = 1.0;} else
        if(world->bots[b].parts[0].eyes[0].str <
        world->bots[b].parts[0].eyes[1].str) {output_1 = 0.0;} else {output_1 =
        0.5;}
        */

        // Advanced movement
        /*
        output_0 = 0.5;
        if(world->bots[b].parts[0].eyes[0].str >
        world->bots[b].parts[0].eyes[1].str)
        {
          // We see a yellow (good) pellet
          if(world->bots[b].parts[0].eyes[0].r > 0.9 &&
        world->bots[b].parts[0].eyes[0].g > 0.9 &&
        world->bots[b].parts[0].eyes[0].b < 0.1)
          {
            output_1 = 1.0;
          }
          // We see a blue (bad) pellet
          else
          {
            output_1 = 0.0;
          }
        }
        else if(world->bots[b].parts[0].eyes[0].str <
        world->bots[b].parts[0].eyes[1].str)
        {
          // We see a yellow (good) pellet
          if(world->bots[b].parts[0].eyes[1].r > 0.9 &&
        world->bots[b].parts[0].eyes[1].g > 0.9 &&
        world->bots[b].parts[0].eyes[1].b < 0.1)
          {
            output_1 = 0.0;
          }
          // We see a blue (bad) pellet
          else
          {
            output_1 = 1.0;
          }
        }
        else
        {
          output_0 = 1.0;
          output_1 = 0.5;
        }
        */

        // FNN movement
        fnn_feedforwards(&world->bots[b].fnn);
        output_0 = world->bots[b]
                       .fnn.neurons[world->bots[b].fnn.num_layers - 1][0]
                       .out;
        output_1 = world->bots[b]
                       .fnn.neurons[world->bots[b].fnn.num_layers - 1][1]
                       .out;

        // Move the head
        world->bots[b].parts[0].angle +=
            (output_1 - 0.5) * world->bots[b].turn_rate;
        NORMALISE_RAD(world->bots[b].parts[0].angle);
        world->bots[b].parts[0].x += 0.1 * output_0 * world->bots[b].speed *
                                     sin(world->bots[b].parts[0].angle);
        world->bots[b].parts[0].y += 0.1 * output_0 * world->bots[b].speed *
                                     cos(world->bots[b].parts[0].angle);

        // Update tail positions
        float spacer = 0.3;
        for (int i = 1; i < world->bots[b].num_parts; ++i) {
            float dx =
                world->bots[b].parts[i - 1].x - world->bots[b].parts[i].x;
            float dy =
                world->bots[b].parts[i - 1].y - world->bots[b].parts[i].y;
            float angle = atan2(dx, dy);

            world->bots[b].parts[i].angle =
                angle + 0.01 * wiggle[(world->frame / 8) % 32];

            NORMALISE_RAD(world->bots[b].parts[i].angle);

            world->bots[b].parts[i].x =
                world->bots[b].parts[i - 1].x -
                spacer * sin(world->bots[b].parts[i].angle);
            world->bots[b].parts[i].y =
                world->bots[b].parts[i - 1].y -
                spacer * cos(world->bots[b].parts[i].angle);

            spacer *= 0.9;
        }

        // Edge detection
        if (world->bots[b].parts[0].x < 0.0) {
            for (int i = 0; i < world->bots[b].num_parts; ++i) {
                world->bots[b].parts[i].x += world->w;
            }
        } else if (world->bots[b].parts[0].x >= world->w) {
            for (int i = 0; i < world->bots[b].num_parts; ++i) {
                world->bots[b].parts[i].x -= world->w;
            }
        }
        if (world->bots[b].parts[0].y < 0.0) {
            for (int i = 0; i < world->bots[b].num_parts; ++i) {
                world->bots[b].parts[i].y += world->h;
            }
        } else if (world->bots[b].parts[0].y >= world->h) {
            for (int i = 0; i < world->bots[b].num_parts; ++i) {
                world->bots[b].parts[i].y -= world->h;
            }
        }

        world->bots[b].age++;
        if (world->bots[b].energy > 0) {
            world->bots[b].energy--;
        } else if (world->bots[b].health > 0) {
            world->bots[b].health--;
        }
    }

    world->frame++;

    // Create the next generation
    if (num_bots_alive <= NUM_PARENTS || world->frame >= GEN_MAX_FRAMES) {
        int bot_number[world->num_bots];
        int bot_score[world->num_bots];

        // Score the bots
        world->average_fitness = 0.0;
        for (int b = 0; b < world->num_bots; ++b) {
            bot_number[b] = b;
            bot_score[b] = world->bots[b].age + world->bots[b].health +
                           world->bots[b].energy;

            world->average_fitness += bot_score[b];
        }
        world->average_fitness = world->average_fitness / world->num_bots;

        // Sort
        for (int a = 0; a < world->num_bots - 1; ++a) {
            for (int b = a + 1; b < world->num_bots; ++b) {
                if (bot_score[b] > bot_score[a]) {
                    int store;

                    store = bot_score[a];
                    bot_score[a] = bot_score[b];
                    bot_score[b] = store;

                    store = bot_number[a];
                    bot_number[a] = bot_number[b];
                    bot_number[b] = store;
                }
            }
        }

        // Create parents
        for (int b = 0; b < NUM_PARENTS; ++b) {
            bot_copy(&world->parents[b], &world->bots[bot_number[b]]);
        }

        // Create new generation
        for (int b = 0; b < world->num_bots; ++b) {
            int parent_num = rand() % NUM_PARENTS;
            int r = bot_copy(&world->bots[b], &world->parents[parent_num]);
            if (r != 0) {
                std::cout << "Uh oh" << std::endl;
            }

            fnn_weights_jiggle(&world->bots[b].fnn);

            world->bots[b].age = 0;
            world->bots[b].energy = BOT_START_ENERGY;
            world->bots[b].health = BOT_START_HEALTH;

            world->bots[b].parts[0].r += RAND_BETWEEN(-0.1, 0.1);
            world->bots[b].parts[0].g += RAND_BETWEEN(-0.1, 0.1);
            world->bots[b].parts[0].b += RAND_BETWEEN(-0.1, 0.1);

            if (world->bots[b].parts[0].r > 1.0) {
                world->bots[b].parts[0].r = 1.0;
            } else if (world->bots[b].parts[0].r < 0.0) {
                world->bots[b].parts[0].r = 0.0;
            }

            if (world->bots[b].parts[0].g > 1.0) {
                world->bots[b].parts[0].g = 1.0;
            } else if (world->bots[b].parts[0].g < 0.0) {
                world->bots[b].parts[0].g = 0.0;
            }

            if (world->bots[b].parts[0].b > 1.0) {
                world->bots[b].parts[0].b = 1.0;
            } else if (world->bots[b].parts[0].b < 0.0) {
                world->bots[b].parts[0].b = 0.0;
            }

            for (int part = 1; part < world->bots[b].num_parts; ++part) {
                world->bots[b].parts[part].r =
                    world->bots[b].parts[part - 1].r -
                    (float)part / world->bots[b].num_parts;
                world->bots[b].parts[part].g =
                    world->bots[b].parts[part - 1].g -
                    (float)part / world->bots[b].num_parts;
                world->bots[b].parts[part].b =
                    world->bots[b].parts[part - 1].b -
                    (float)part / world->bots[b].num_parts;
            }

            world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2.0 * M_PI);
            world->bots[b].parts[0].x = RAND_BETWEEN(0.0, world->w);
            world->bots[b].parts[0].y = RAND_BETWEEN(0.0, world->h);
        }

        // Replace all the pellets
        for (int p = 0; p < world->num_pellets; ++p) {
            world_pellet_replace(world, p);
        }

        // Log stats
        FILE *file = fopen("results.txt", "a");
        if (file) {
            fprintf(file,
                    "%i\t%i\t%i\t%.2f\n",
                    world->generation,
                    world->pellets_eaten_good,
                    world->pellets_eaten_bad,
                    world->average_fitness);
            fclose(file);
        }

        // Reset stats
        world->pellets_eaten_good = 0;
        world->pellets_eaten_bad = 0;

        world->frame = 0;
        world->generation++;
    }

    return 0;
}

int world_bots_add(World *world, int n) {
    assert(world);

    for (int i = 0; i < n; ++i) {
        int r = world_bot_add(world);
        if (r != 0) {
            return r;
        }
    }

    return 0;
}

int world_bot_add(World *world) {
    assert(world);

    if (world->num_bots >= MAX_BOTS) {
        return -1;
    }

    int b = world->num_bots;

    static int id = 0;

    float x = RAND_BETWEEN(0.0, world->w);
    float y = RAND_BETWEEN(0.0, world->h);
    float red = RAND_BETWEEN(0.0, 1.0);
    float green = RAND_BETWEEN(0.0, 1.0);
    float blue = RAND_BETWEEN(0.0, 1.0);

    float spacer = 0.25;
    world->bots[b].id = id;
    world->bots[b].age = 0;
    world->bots[b].energy = BOT_START_ENERGY;
    world->bots[b].health = BOT_START_HEALTH;
    world->bots[b].speed = 1.0;
    world->bots[b].turn_rate = DEG_TO_RAD(3.0);
    world->bots[b].num_parts = MAX_PARTS - rand() % 3;
    world->bots[b].total_eyes = 0;
    world->bots[b].total_spikes = 0;
    world->bots[b].total_ears = 0;

    // Neural network
    fnn_weights_randomise(&world->bots[b].fnn);

    if (world->bots[b].num_parts < 1) {
        world->bots[b].num_parts = 1;
    } else if (world->bots[b].num_parts > MAX_PARTS) {
        world->bots[b].num_parts = MAX_PARTS;
    }

    // Body parts
    for (int part = 0; part < world->bots[b].num_parts; ++part) {
        if (part == 0) {
            world->bots[b].parts[0].angle = RAND_BETWEEN(0.0, 2.0 * M_PI);
            world->bots[b].parts[0].x = x;
            world->bots[b].parts[0].y = y;
        } else {
            world->bots[b].parts[part].angle =
                world->bots[b].parts[part - 1].angle +
                RAND_BETWEEN(-2.0 * M_PI / 16, 2.0 * M_PI / 16);
            world->bots[b].parts[part].x =
                world->bots[b].parts[part - 1].x -
                spacer * sin(world->bots[b].parts[part].angle);
            world->bots[b].parts[part].y =
                world->bots[b].parts[part - 1].y -
                spacer * cos(world->bots[b].parts[part].angle);

            spacer *= 0.9;
        }
        NORMALISE_RAD(world->bots[b].parts[part].angle);

        world->bots[b].parts[part].num_eyes = 0;
        world->bots[b].parts[part].num_spikes = 0;
        world->bots[b].parts[part].num_ears = 0;
        world->bots[b].parts[part].radius = spacer;
        world->bots[b].parts[part].r =
            red - (float)part / world->bots[b].num_parts;
        world->bots[b].parts[part].g =
            green - (float)part / world->bots[b].num_parts;
        world->bots[b].parts[part].b =
            blue - (float)part / world->bots[b].num_parts;
    }

    float multiplier;

    // Eyes
    /*
    multiplier = 1.0;
    bot_eye_add(&world->bots[b], 0,                          DEG_TO_RAD(15),
    DEG_TO_RAD(60), 2.5*multiplier); bot_eye_add(&world->bots[b], 0,
    DEG_TO_RAD(345), DEG_TO_RAD(60), 2.5*multiplier);
    bot_eye_add(&world->bots[b], world->bots[b].num_parts-1, DEG_TO_RAD(180),
    DEG_TO_RAD(30), 2.5*multiplier);
    */
    multiplier = RAND_BETWEEN(0.8, 1.0);
    bot_eye_add(&world->bots[b],
                0,
                1 * (2.0 * M_PI / 12),
                2.0 * M_PI / 5,
                2.5 * multiplier);
    bot_eye_add(&world->bots[b],
                0,
                11 * (2.0 * M_PI / 12),
                2.0 * M_PI / 5,
                2.5 * multiplier);
    bot_eye_add(&world->bots[b],
                world->bots[b].num_parts - 1,
                M_PI,
                2.0 * M_PI / 4,
                1.5 * multiplier);

    // Spikes
    multiplier = RAND_BETWEEN(0.8, 1.0);
    if (MAX_PARTS == 1) {
        bot_spike_add(&world->bots[b], 0, 0.8 * multiplier, 0.0);
    } else {
        // (Caterpillar look)
        bot_spike_add(&world->bots[b], 0, 0.8 * multiplier, 0.0);
        bot_spike_add(
            &world->bots[b], 0, 0.6 * multiplier, 5 * (2.0 * M_PI / 6));
        bot_spike_add(
            &world->bots[b], 0, 0.6 * multiplier, 1 * (2.0 * M_PI / 6));
        bot_spike_add(
            &world->bots[b], 1, 0.6 * multiplier, 5 * (2.0 * M_PI / 6));
        bot_spike_add(
            &world->bots[b], 1, 0.6 * multiplier, 1 * (2.0 * M_PI / 6));
        for (int i = 1; i < world->bots[b].num_parts - 1; ++i) {
            bot_spike_add(&world->bots[b],
                          i,
                          2.0 * multiplier * world->bots[b].parts[i].radius,
                          4 * (2.0 * M_PI / 6));
            bot_spike_add(&world->bots[b],
                          i,
                          2.0 * multiplier * world->bots[b].parts[i].radius,
                          2 * (2.0 * M_PI / 6));
        }
        bot_spike_add(
            &world->bots[b],
            world->bots[b].num_parts - 1,
            3.0 * multiplier *
                world->bots[b].parts[world->bots[b].num_parts - 1].radius,
            4 * (2.0 * M_PI / 6));
        bot_spike_add(
            &world->bots[b],
            world->bots[b].num_parts - 1,
            3.0 * multiplier *
                world->bots[b].parts[world->bots[b].num_parts - 1].radius,
            2 * (2.0 * M_PI / 6));
        bot_spike_add(
            &world->bots[b],
            world->bots[b].num_parts - 1,
            5.0 * multiplier *
                world->bots[b].parts[world->bots[b].num_parts - 1].radius,
            2.0 * M_PI / 2);
    }

    // Ears
    multiplier = RAND_BETWEEN(0.8, 1.0);
    bot_ear_add(&world->bots[b], 0, 5.0 * multiplier);

    world->num_bots++;
    id++;

    return 0;
}

int world_pellet_remove(World *world, int p) {
    assert(world);

    if (p >= world->num_pellets) {
        return -1;
    }

    world->pellets[p] = world->pellets[world->num_pellets - 1];
    world->num_pellets--;

    return 0;
}

int world_pellets_add(World *world, int n) {
    assert(world);

    for (int i = 0; i < n; ++i) {
        int r = world_pellet_add(world);
        if (r != 0) {
            return r;
        }
    }

    return 0;
}

int world_pellet_replace(World *world, int p) {
    assert(world);

    if (p < 0) {
        return -1;
    } else if (p > world->num_pellets) {
        return -2;
    }

    world->pellets[p].x = RAND_BETWEEN(0, world->w);
    world->pellets[p].y = RAND_BETWEEN(0, world->h);

    return 0;
}

int world_pellet_add(World *world) {
    assert(world);

    if (world->num_pellets >= MAX_PELLETS) {
        return -1;
    }

    int i = world->num_pellets;

    static int id = 0;

    world->pellets[i].id = id;

    world->pellets[i].x = RAND_BETWEEN(0, world->w);
    world->pellets[i].y = RAND_BETWEEN(0, world->h);
    world->pellets[i].size = 0.2;

    if (rand() % 10 != 0)  // Good pellets
    {
        world->pellets[i].energy = PELLET_ENERGY;
        // Yellow
        world->pellets[i].r = 1.0;
        world->pellets[i].g = 1.0;
        world->pellets[i].b = 0.0;
    } else  // Bad pellets
    {
        world->pellets[i].energy = -2 * PELLET_ENERGY;
        // Blue
        world->pellets[i].r = 0.0;
        world->pellets[i].g = 0.0;
        world->pellets[i].b = 1.0;
    }

    world->num_pellets++;
    id++;

    return 0;
}

int world_print_details(World *world) {
    assert(world);

    std::cout << "--- World ---" << std::endl;
    std::cout << "Seed: " << world->seed << std::endl;
    std::cout << "Frame: " << world->frame << std::endl;
    std::cout << "Generation: " << world->generation << std::endl;
    std::cout << "Width: " << world->w << std::endl;
    std::cout << "Height: " << world->h << std::endl;
    std::cout << "Num bots: " << world->num_bots << std::endl;
    std::cout << "Num pellets: " << world->num_pellets << std::endl;
    std::cout << "Grid w: " << world->grid_w << std::endl;
    std::cout << "Grid h: " << world->grid_h << std::endl;
    std::cout << std::endl;

    return 0;
}

int world_init(World *world) {
    assert(world);

    world->seed = time(0);
    srand(world->seed);
    world->frame = 0;
    world->generation = 0;
    world->w = 96.0;
    world->h = 54.0;
    world->num_bots = 0;
    world->num_pellets = 0;
    world->average_fitness = 0.0;
    world->grid_w = 16;
    world->grid_h = 9;

    FILE *file = fopen("results.txt", "w");
    if (file) {
        fclose(file);
    }

    // Stats
    world->pellets_eaten_good = 0;
    world->pellets_eaten_bad = 0;

    // Create the bot brains
    int sizes[4] = {14, 8, 8, 2};
    for (int b = 0; b < MAX_BOTS; ++b) {
        fnn_init(&world->bots[b].fnn, 4, sizes);
    }
    for (int b = 0; b < NUM_PARENTS; ++b) {
        fnn_init(&world->parents[b].fnn, 4, sizes);
    }

    world->grid = (Tile **)malloc(world->grid_w * sizeof(Tile *));

    for (int w = 0; w < world->grid_w; ++w) {
        world->grid[w] = (Tile *)malloc(world->grid_h * sizeof(Tile));
        for (int h = 0; h < world->grid_h; ++h) {
            world->grid[w][h].food = 100;
            // Green-Brown
            // world->grid[w][h].r = RAND_BETWEEN(0.65, 0.75);
            // world->grid[w][h].g = RAND_BETWEEN(0.55, 0.65);
            // world->grid[w][h].b = RAND_BETWEEN(0.25, 0.35);
            // Green
            world->grid[w][h].r = RAND_BETWEEN(0.2, 0.3);
            world->grid[w][h].g = RAND_BETWEEN(0.4, 0.7);
            world->grid[w][h].b = RAND_BETWEEN(0.1, 0.2);
        }
    }

    return 0;
}
