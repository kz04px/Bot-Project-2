#include "buffers.hpp"
#include <cassert>
#include <cmath>

int buffers_fill_bots(World *world) {
    assert(world);

    return 0;
}

int buffers_init_bots(Buffers *buffers) {
    assert(buffers);

    glGenVertexArrays(1, &buffers->vao);
    glBindVertexArray(buffers->vao);

    // Vertices
    glGenBuffers(1, &buffers->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    // Positions
    glGenBuffers(1, &buffers->tbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    // Colours
    glGenBuffers(1, &buffers->cbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    // Rotation
    glGenBuffers(1, &buffers->rbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 1, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    return 0;
}

int buffers_init_pellets(Buffers *buffers) {
    assert(buffers);

    glGenVertexArrays(1, &buffers->vao);
    glBindVertexArray(buffers->vao);

    // Indices
    glGenBuffers(1, &buffers->ibo);

    // Vertices
    glGenBuffers(1, &buffers->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    // Positions
    glGenBuffers(1, &buffers->tbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));
    glVertexAttribDivisor(1, 1);

    // Colours
    glGenBuffers(1, &buffers->cbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));
    glVertexAttribDivisor(2, 1);

    return 0;
}

int buffers_fill_pellets(World *world, Buffers *buffers) {
    assert(world);
    assert(buffers);

    float pellet_points[8];

    glBindVertexArray(buffers->vao);

    // Vertices
    float size = 0.1;
    pellet_points[0] = -size;  // p1
    pellet_points[1] = -size;
    pellet_points[2] = -size;  // p2
    pellet_points[3] = size;
    pellet_points[4] = size;  // p3
    pellet_points[5] = size;
    pellet_points[6] = size;  // p4
    pellet_points[7] = -size;
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 4 * sizeof *pellet_points,
                 pellet_points,
                 GL_STATIC_DRAW);

    // Indices
    GLuint indexData[] = {0, 1, 2, 0, 2, 3};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 8 * sizeof(*indexData),
                 indexData,
                 GL_STATIC_DRAW);

    // Positions
    GLfloat translationData[2 * world->num_pellets];
    for (int p = 0; p < world->num_pellets; ++p) {
        translationData[2 * p + 0] = world->pellets[p].x;
        translationData[2 * p + 1] = world->pellets[p].y;
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * world->num_pellets * sizeof(*translationData),
                 translationData,
                 GL_STATIC_DRAW);

    // Colours
    GLfloat colourData[3 * world->num_pellets];
    for (int p = 0; p < world->num_pellets; ++p) {
        colourData[3 * p + 0] = world->pellets[p].r;
        colourData[3 * p + 1] = world->pellets[p].g;
        colourData[3 * p + 2] = world->pellets[p].b;
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * world->num_pellets * sizeof(*colourData),
                 colourData,
                 GL_STATIC_DRAW);

    return 0;
}

int buffers_init_background(Buffers *buffers) {
    assert(buffers);

    glGenVertexArrays(1, &buffers->vao);
    glBindVertexArray(buffers->vao);

    // Indices
    glGenBuffers(1, &buffers->ibo);

    // Vertices
    glGenBuffers(1, &buffers->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));

    // Positions
    glGenBuffers(1, &buffers->tbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));
    glVertexAttribDivisor(1, 1);

    // Colours
    glGenBuffers(1, &buffers->cbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + 0 * sizeof(GLfloat));
    glVertexAttribDivisor(2, 1);

    return 0;
}

int buffers_fill_background(World *world, Buffers *buffers) {
    assert(world);
    assert(buffers);

    float background_points[12];
    int i = 0;

    float angle = 2.0 * M_PI / 6.0;
    float hangle = angle / 2.0;
    float r_y = 0.5 * (world->h / world->grid_h) / 0.866;
    float r_x = (world->w / world->grid_w) - r_y * sin(hangle);

    // Vertices
    background_points[0] = r_x * sin(0 * angle + hangle);  // p1
    background_points[1] = r_y * cos(0 * angle + hangle);
    background_points[2] = r_x * sin(1 * angle + hangle);  // p2
    background_points[3] = r_y * cos(1 * angle + hangle);
    background_points[4] = r_x * sin(2 * angle + hangle);  // p3
    background_points[5] = r_y * cos(2 * angle + hangle);
    background_points[6] = r_x * sin(3 * angle + hangle);  // p4
    background_points[7] = r_y * cos(3 * angle + hangle);
    background_points[8] = r_x * sin(4 * angle + hangle);  // p5
    background_points[9] = r_y * cos(4 * angle + hangle);
    background_points[10] = r_x * sin(5 * angle + hangle);  // p6
    background_points[11] = r_y * cos(5 * angle + hangle);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 12 * sizeof *background_points,
                 background_points,
                 GL_STATIC_DRAW);

    // Indices
    GLuint indexData[] = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * 12,
                 indexData,
                 GL_STATIC_DRAW);

    // Positions
    i = 0;
    GLfloat translationData[2 * world->grid_w * world->grid_h];
    for (int w = 0; w < world->grid_w; ++w) {
        for (int h = 0; h < world->grid_h; ++h) {
            translationData[2 * i + 0] = (float)w / world->grid_w * world->w;
            translationData[2 * i + 1] =
                (float)h / world->grid_h * world->h +
                0.5 * (w % 2) * (world->h / world->grid_h);
            i++;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * 2 * world->grid_w * world->grid_h,
                 translationData,
                 GL_STATIC_DRAW);

    // Colours
    i = 0;
    GLfloat colourData[3 * world->grid_w * world->grid_h];
    for (int w = 0; w < world->grid_w; ++w) {
        for (int h = 0; h < world->grid_h; ++h) {
            colourData[3 * i + 0] = world->grid[w][h].r;
            colourData[3 * i + 1] = world->grid[w][h].g;
            colourData[3 * i + 2] = world->grid[w][h].b;
            i++;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * 3 * world->grid_w * world->grid_h,
                 colourData,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_body(Part *part, Buffers *buffers, float scale) {
    assert(part);
    assert(buffers);

    float bot_vertices[2 * 5];
    float bot_positions[2 * 5];
    float bot_colours[3 * 5];
    float bot_rotations[3 * 5];

    float angle = 2.0 * M_PI / 5;
    float size = 0.25 * scale;

    // p1
    bot_vertices[0] = size * sin(0 * angle);
    bot_vertices[1] = size * cos(0 * angle);
    bot_positions[0] = part->x;
    bot_positions[1] = part->y;
    bot_colours[0] = part->r;
    bot_colours[1] = part->g;
    bot_colours[2] = part->b;
    bot_rotations[0] = part->angle;
    // p2
    bot_vertices[2] = size * sin(1 * angle);
    bot_vertices[3] = size * cos(1 * angle);
    bot_positions[2] = part->x;
    bot_positions[3] = part->y;
    bot_colours[3] = part->r;
    bot_colours[4] = part->g;
    bot_colours[5] = part->b;
    bot_rotations[1] = part->angle;
    // p3
    bot_vertices[4] = size * sin(2 * angle);
    bot_vertices[5] = size * cos(2 * angle);
    bot_positions[4] = part->x;
    bot_positions[5] = part->y;
    bot_colours[6] = part->r;
    bot_colours[7] = part->g;
    bot_colours[8] = part->b;
    bot_rotations[2] = part->angle;
    // p4
    bot_vertices[6] = size * sin(3 * angle);
    bot_vertices[7] = size * cos(3 * angle);
    bot_positions[6] = part->x;
    bot_positions[7] = part->y;
    bot_colours[9] = part->r;
    bot_colours[10] = part->g;
    bot_colours[11] = part->b;
    bot_rotations[3] = part->angle;
    // p5
    bot_vertices[8] = size * sin(4 * angle);
    bot_vertices[9] = size * cos(4 * angle);
    bot_positions[8] = part->x;
    bot_positions[9] = part->y;
    bot_colours[12] = part->r;
    bot_colours[13] = part->g;
    bot_colours[14] = part->b;
    bot_rotations[4] = part->angle;

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 5 * sizeof *bot_vertices,
                 bot_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 5 * sizeof *bot_positions,
                 bot_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 5 * sizeof *bot_colours,
                 bot_colours,
                 GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 5 * sizeof *bot_rotations,
                 bot_rotations,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_spikes(Part *part, Buffers *buffers) {
    assert(part);
    assert(buffers);

    if (part->num_spikes == 0) {
        return 0;
    }

    float spike_vertices[2 * 3 * part->num_spikes];
    float spike_positions[2 * 3 * part->num_spikes];
    float spike_colours[3 * 3 * part->num_spikes];
    float spike_rotations[1 * 3 * part->num_spikes];

    for (int s = 0; s < part->num_spikes; ++s) {
        // p0
        spike_vertices[2 * 3 * s + 0] = 0.0;
        spike_vertices[2 * 3 * s + 1] = part->spikes[s].length;
        spike_positions[2 * 3 * s + 0] = part->x;
        spike_positions[2 * 3 * s + 1] = part->y;
        spike_colours[3 * 3 * s + 0] = part->spikes[s].r;
        spike_colours[3 * 3 * s + 1] = part->spikes[s].g;
        spike_colours[3 * 3 * s + 2] = part->spikes[s].b;
        spike_rotations[1 * 3 * s + 0] = part->angle + part->spikes[s].angle;

        // p1
        spike_vertices[2 * 3 * s + 2] = 0.025;
        spike_vertices[2 * 3 * s + 3] = 0.5 * part->radius;
        spike_positions[2 * 3 * s + 2] = part->x;
        spike_positions[2 * 3 * s + 3] = part->y;
        spike_colours[3 * 3 * s + 3] = part->spikes[s].r;
        spike_colours[3 * 3 * s + 4] = part->spikes[s].g;
        spike_colours[3 * 3 * s + 5] = part->spikes[s].b;
        spike_rotations[1 * 3 * s + 1] = part->angle + part->spikes[s].angle;

        // p2
        spike_vertices[2 * 3 * s + 4] = -0.025;
        spike_vertices[2 * 3 * s + 5] = 0.5 * part->radius;
        spike_positions[2 * 3 * s + 4] = part->x;
        spike_positions[2 * 3 * s + 5] = part->y;
        spike_colours[3 * 3 * s + 6] = part->spikes[s].r;
        spike_colours[3 * 3 * s + 7] = part->spikes[s].g;
        spike_colours[3 * 3 * s + 8] = part->spikes[s].b;
        spike_rotations[1 * 3 * s + 2] = part->angle + part->spikes[s].angle;
    }

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 3 * part->num_spikes * sizeof *spike_vertices,
                 spike_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 3 * part->num_spikes * sizeof *spike_positions,
                 spike_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 3 * part->num_spikes * sizeof *spike_colours,
                 spike_colours,
                 GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 1 * 3 * part->num_spikes * sizeof *spike_rotations,
                 spike_rotations,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_ears(Part *part, Buffers *buffers) {
    assert(part);
    assert(buffers);

    if (part->num_ears == 0) {
        return 0;
    }

    float ear_range_vertices[2 * 2 * EAR_RANGE_ACCURACY * part->num_ears];
    float ear_range_positions[2 * 2 * EAR_RANGE_ACCURACY * part->num_ears];
    float ear_range_colours[3 * 2 * EAR_RANGE_ACCURACY * part->num_ears];
    float ear_range_rotations[1 * 2 * EAR_RANGE_ACCURACY * part->num_ears];

    float angle = 2.0 * M_PI / EAR_RANGE_ACCURACY;

    for (int ear = 0; ear < part->num_ears; ++ear) {
        for (int i = 0; i < EAR_RANGE_ACCURACY; ++i) {
            // pn
            ear_range_vertices[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 0] =
                part->ears[ear].dist * sin(i * angle);
            ear_range_vertices[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 1] =
                part->ears[ear].dist * cos(i * angle);
            ear_range_positions[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 0] =
                part->x;
            ear_range_positions[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 1] =
                part->y;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 0] =
                part->ears[ear].str;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 1] = 0.0;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 2] = 0.0;
            ear_range_rotations[2 * (EAR_RANGE_ACCURACY)*ear + 2 * i + 0] =
                part->angle;
            // pn+1
            ear_range_vertices[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 2] =
                part->ears[ear].dist * sin((i + 1) * angle);
            ear_range_vertices[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 3] =
                part->ears[ear].dist * cos((i + 1) * angle);
            ear_range_positions[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 2] =
                part->x;
            ear_range_positions[4 * (EAR_RANGE_ACCURACY)*ear + 4 * i + 3] =
                part->y;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 3] =
                part->ears[ear].str;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 4] = 0.0;
            ear_range_colours[6 * (EAR_RANGE_ACCURACY)*ear + 6 * i + 5] = 0.0;
            ear_range_rotations[2 * (EAR_RANGE_ACCURACY)*ear + 2 * i + 1] =
                part->angle;
        }
    }

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 4 * EAR_RANGE_ACCURACY * part->num_ears *
                     sizeof *ear_range_vertices,
                 ear_range_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 4 * EAR_RANGE_ACCURACY * part->num_ears *
                     sizeof *ear_range_positions,
                 ear_range_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        3 * 6 * EAR_RANGE_ACCURACY * part->num_ears * sizeof *ear_range_colours,
        ear_range_colours,
        GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 2 * EAR_RANGE_ACCURACY * part->num_ears *
                     sizeof *ear_range_rotations,
                 ear_range_rotations,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_eyes(Part *part, Buffers *buffers) {
    assert(part);
    assert(buffers);

    if (part->num_eyes == 0) {
        return 0;
    }

    float eye_size = 0.02;
    float eye_vertices[2 * 4 * part->num_eyes];
    float eye_positions[2 * 4 * part->num_eyes];
    float eye_colours[3 * 4 * part->num_eyes];
    float eye_rotations[1 * 4 * part->num_eyes];

    for (int e = 0; e < part->num_eyes; ++e) {
        // p0
        eye_vertices[2 * 4 * e + 0] = -eye_size;
        eye_vertices[2 * 4 * e + 1] = -eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 0] = part->x;
        eye_positions[2 * 4 * e + 1] = part->y;
        eye_colours[3 * 4 * e + 0] = part->eyes[e].r;
        eye_colours[3 * 4 * e + 1] = part->eyes[e].g;
        eye_colours[3 * 4 * e + 2] = part->eyes[e].b;
        eye_rotations[1 * 4 * e + 0] = part->angle + part->eyes[e].angle;

        // p1
        eye_vertices[2 * 4 * e + 2] = -eye_size;
        eye_vertices[2 * 4 * e + 3] = eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 2] = part->x;
        eye_positions[2 * 4 * e + 3] = part->y;
        eye_colours[3 * 4 * e + 3] = part->eyes[e].r;
        eye_colours[3 * 4 * e + 4] = part->eyes[e].g;
        eye_colours[3 * 4 * e + 5] = part->eyes[e].b;
        eye_rotations[1 * 4 * e + 1] = part->angle + part->eyes[e].angle;

        // p2
        eye_vertices[2 * 4 * e + 4] = eye_size;
        eye_vertices[2 * 4 * e + 5] = eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 4] = part->x;
        eye_positions[2 * 4 * e + 5] = part->y;
        eye_colours[3 * 4 * e + 6] = part->eyes[e].r;
        eye_colours[3 * 4 * e + 7] = part->eyes[e].g;
        eye_colours[3 * 4 * e + 8] = part->eyes[e].b;
        eye_rotations[1 * 4 * e + 2] = part->angle + part->eyes[e].angle;

        // p3
        eye_vertices[2 * 4 * e + 6] = eye_size;
        eye_vertices[2 * 4 * e + 7] = -eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 6] = part->x;
        eye_positions[2 * 4 * e + 7] = part->y;
        eye_colours[3 * 4 * e + 9] = part->eyes[e].r;
        eye_colours[3 * 4 * e + 10] = part->eyes[e].g;
        eye_colours[3 * 4 * e + 11] = part->eyes[e].b;
        eye_rotations[1 * 4 * e + 3] = part->angle + part->eyes[e].angle;
    }

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 4 * part->num_eyes * sizeof *eye_vertices,
                 eye_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 4 * part->num_eyes * sizeof *eye_positions,
                 eye_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 4 * part->num_eyes * sizeof *eye_colours,
                 eye_colours,
                 GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 1 * 4 * part->num_eyes * sizeof *eye_rotations,
                 eye_rotations,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_eye_borders(Part *part, Buffers *buffers) {
    assert(part);
    assert(buffers);

    if (part->num_eyes == 0) {
        return 0;
    }

    float eye_size = 0.02;
    float eye_vertices[2 * 4 * part->num_eyes];
    float eye_positions[2 * 4 * part->num_eyes];
    float eye_colours[3 * 4 * part->num_eyes];
    float eye_rotations[1 * 4 * part->num_eyes];

    for (int e = 0; e < part->num_eyes; ++e) {
        // p0
        eye_vertices[2 * 4 * e + 0] = -1.2 * eye_size;
        eye_vertices[2 * 4 * e + 1] = -1.2 * eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 0] = part->x;
        eye_positions[2 * 4 * e + 1] = part->y;
        eye_colours[3 * 4 * e + 0] = 0.0;
        eye_colours[3 * 4 * e + 1] = 0.0;
        eye_colours[3 * 4 * e + 2] = 0.0;
        eye_rotations[1 * 4 * e + 0] = part->angle + part->eyes[e].angle;

        // p1
        eye_vertices[2 * 4 * e + 2] = -1.2 * eye_size;
        eye_vertices[2 * 4 * e + 3] = 1.2 * eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 2] = part->x;
        eye_positions[2 * 4 * e + 3] = part->y;
        eye_colours[3 * 4 * e + 3] = 0.0;
        eye_colours[3 * 4 * e + 4] = 0.0;
        eye_colours[3 * 4 * e + 5] = 0.0;
        eye_rotations[1 * 4 * e + 1] = part->angle + part->eyes[e].angle;

        // p2
        eye_vertices[2 * 4 * e + 4] = 1.2 * eye_size;
        eye_vertices[2 * 4 * e + 5] = 1.2 * eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 4] = part->x;
        eye_positions[2 * 4 * e + 5] = part->y;
        eye_colours[3 * 4 * e + 6] = 0.0;
        eye_colours[3 * 4 * e + 7] = 0.0;
        eye_colours[3 * 4 * e + 8] = 0.0;
        eye_rotations[1 * 4 * e + 2] = part->angle + part->eyes[e].angle;

        // p3
        eye_vertices[2 * 4 * e + 6] = 1.2 * eye_size;
        eye_vertices[2 * 4 * e + 7] = -1.2 * eye_size + 0.6 * part->radius;
        eye_positions[2 * 4 * e + 6] = part->x;
        eye_positions[2 * 4 * e + 7] = part->y;
        eye_colours[3 * 4 * e + 9] = 0.0;
        eye_colours[3 * 4 * e + 10] = 0.0;
        eye_colours[3 * 4 * e + 11] = 0.0;
        eye_rotations[1 * 4 * e + 3] = part->angle + part->eyes[e].angle;
    }

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 4 * part->num_eyes * sizeof *eye_vertices,
                 eye_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * 4 * part->num_eyes * sizeof *eye_positions,
                 eye_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3 * 4 * part->num_eyes * sizeof *eye_colours,
                 eye_colours,
                 GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 1 * 4 * part->num_eyes * sizeof *eye_rotations,
                 eye_rotations,
                 GL_STATIC_DRAW);

    return 0;
}

int buffer_fill_bot_cones(Part *part, Buffers *buffers) {
    assert(part);
    assert(buffers);

    if (part->num_eyes == 0) {
        return 0;
    }

    float eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * part->num_eyes];
    float eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * part->num_eyes];
    float eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * part->num_eyes];
    float eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * part->num_eyes];

    for (int e = 0; e < part->num_eyes; ++e) {
        // p0
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 0] = 0.0;
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 1] = 0.0;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 0] =
            part->x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 1] =
            part->y;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 0] =
            part->eyes[e].r;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 1] =
            part->eyes[e].g;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 2] =
            part->eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (0) + 0] = 0.0;
        // p1
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 2] = 0.0;
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 3] =
            part->eyes[e].dist;
        ;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 2] =
            part->x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (0) + 3] =
            part->y;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 3] =
            part->eyes[e].r;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 4] =
            part->eyes[e].g;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (0) + 5] =
            part->eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (0) + 1] =
            part->angle + part->eyes[e].angle - part->eyes[e].fov / 2;
        // p3
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 0] = 0.0;
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 1] = 0.0;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 0] =
            part->x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 1] =
            part->y;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 0] =
            part->eyes[e].r;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 1] =
            part->eyes[e].g;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 2] =
            part->eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (1) + 0] = 0.0;
        // p4
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 2] = 0.0;
        eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 3] =
            part->eyes[e].dist;
        ;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 2] =
            part->x;
        eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (1) + 3] =
            part->y;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 3] =
            part->eyes[e].r;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 4] =
            part->eyes[e].g;
        eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (1) + 5] =
            part->eyes[e].b;
        eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (1) + 1] =
            part->angle + part->eyes[e].angle + part->eyes[e].fov / 2;

        for (int n = 0; n < EYE_CONE_ACCURACY; ++n) {
            // pn
            eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                              0] = 0.0;
            eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                              1] = part->eyes[e].dist;
            eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                               0] = part->x;
            eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                               1] = part->y;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             0] = part->eyes[e].r;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             1] = part->eyes[e].g;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             2] = part->eyes[e].b;
            eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (n + 2) +
                               0] =
                part->angle + part->eyes[e].angle - part->eyes[e].fov / 2 +
                (float)n * (part->eyes[e].fov / EYE_CONE_ACCURACY);
            // pn+1
            eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                              2] = 0.0;
            eye_cone_vertices[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                              3] = part->eyes[e].dist;
            eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                               2] = part->x;
            eye_cone_positions[4 * (EYE_CONE_ACCURACY + 2) * e + 4 * (n + 2) +
                               3] = part->y;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             3] = part->eyes[e].r;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             4] = part->eyes[e].g;
            eye_cone_colours[6 * (EYE_CONE_ACCURACY + 2) * e + 6 * (n + 2) +
                             5] = part->eyes[e].b;
            eye_cone_rotations[2 * (EYE_CONE_ACCURACY + 2) * e + 2 * (n + 2) +
                               1] =
                part->angle + part->eyes[e].angle - part->eyes[e].fov / 2 +
                (float)(n + 1) * (part->eyes[e].fov / EYE_CONE_ACCURACY);
        }
    }

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 4 * (EYE_CONE_ACCURACY + 2) * part->num_eyes *
                     sizeof *eye_cone_vertices,
                 eye_cone_vertices,
                 GL_STATIC_DRAW);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, buffers->tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 4 * (EYE_CONE_ACCURACY + 2) * part->num_eyes *
                     sizeof *eye_cone_positions,
                 eye_cone_positions,
                 GL_STATIC_DRAW);

    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, buffers->cbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        6 * (EYE_CONE_ACCURACY + 2) * part->num_eyes * sizeof *eye_cone_colours,
        eye_cone_colours,
        GL_STATIC_DRAW);

    // Rotations
    glBindBuffer(GL_ARRAY_BUFFER, buffers->rbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2 * (EYE_CONE_ACCURACY + 2) * part->num_eyes *
                     sizeof *eye_cone_rotations,
                 eye_cone_rotations,
                 GL_STATIC_DRAW);

    return 0;
}
