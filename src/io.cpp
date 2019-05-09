#include "io.hpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "defs.hpp"

int print_log(const char *format, ...) {
    FILE *file = fopen("log.txt", "a");
    if (!file) {
        return -1;
    }

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
    return 0;
}

void print_log_shader_info(GLuint shader_index) {
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
    print_log("Shader info log for GL index %i: %s\n", shader_index, log);
}

int screenshot_tga(char *path, int w, int h) {
    assert(path);

    unsigned char *pixels = (unsigned char *)malloc(3 * w * h * sizeof *pixels);
    if (!pixels) {
        return -1;
    }

    glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char header[6] = {((int)(w % 256)),
                               ((int)(w / 256)),
                               ((int)(h % 256)),
                               ((int)(h / 256)),
                               24,
                               0};

    FILE *file = fopen(path, "wb");
    if (!file) {
        return -2;
    }
    fwrite(TGAheader, sizeof *TGAheader, 12, file);
    fwrite(header, sizeof *header, 6, file);
    fwrite(pixels, sizeof *pixels, 3 * w * h, file);
    fclose(file);
    return 0;
}
