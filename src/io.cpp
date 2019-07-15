#include "io.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
