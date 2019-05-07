#include "matrix.hpp"
#include <stdio.h>
#include <string.h>
#include <cmath>

#define EPSILON 0.0000001
#define DEG_TO_RAD(x) (x * 0.0174532925)
#define RAD_TO_DEG(x) (x / 0.0174532925)

Mat4 identity_matrix() {
    Mat4 mat;
    mat.m[0] = 1.0;
    mat.m[1] = 0.0;
    mat.m[2] = 0.0;
    mat.m[3] = 0.0;
    mat.m[4] = 0.0;
    mat.m[5] = 1.0;
    mat.m[6] = 0.0;
    mat.m[7] = 0.0;
    mat.m[8] = 0.0;
    mat.m[9] = 0.0;
    mat.m[10] = 1.0;
    mat.m[11] = 0.0;
    mat.m[12] = 0.0;
    mat.m[13] = 0.0;
    mat.m[14] = 0.0;
    mat.m[15] = 1.0;
    return mat;
}

Vec4 vec4(float i, float j, float k, float l) {
    Vec4 vec;
    vec.i = i;
    vec.j = j;
    vec.k = k;
    vec.l = l;
    return vec;
}

Vec3 vec3(float i, float j, float k) {
    Vec3 vec;
    vec.i = i;
    vec.j = j;
    vec.k = k;
    return vec;
}

Vec2 vec2(float i, float j) {
    Vec2 vec;
    vec.i = i;
    vec.j = j;
    return vec;
}

int line_line_intersection(Vec2 *val,
                           Vec2 w1_a,
                           Vec2 w1_b,
                           Vec2 w2_a,
                           Vec2 w2_b) {
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
    // Store the values for fast access and easy
    // equations-to-code conversion
    float x1 = w1_a.i;
    float x2 = w1_b.i;
    float x3 = w2_a.i;
    float x4 = w2_b.i;
    float y1 = w1_a.j;
    float y2 = w1_b.j;
    float y3 = w2_a.j;
    float y4 = w2_b.j;

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    if (d == 0) {
        return 1;
    }

    // Get the x and y
    float pre = (x1 * y2 - y1 * x2);
    float post = (x3 * y4 - y3 * x4);
    float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
    float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

    // Check if the x and y coordinates are within both lines
    if (x < min(x1, x2) || x > max(x1, x2) || x < min(x3, x4) ||
        x > max(x3, x4)) {
        return 2;
    }
    if (y < min(y1, y2) || y > max(y1, y2) || y < min(y3, y4) ||
        y > max(y3, y4)) {
        return 3;
    }

    // Return the point of intersection
    val->i = x;
    val->j = y;
    return 0;

    /*
    float a = (w2_b.i - w2_a.i)*(w1_a.j - w2_a.j) - (w2_b.j - w2_a.j)*(w1_a.i -
    w2_a.i); float b = (w1_b.i - w1_a.i)*(w1_a.j - w2_a.j) - (w1_b.j -
    w1_a.j)*(w1_a.i - w2_a.i); float c = (w2_b.j - w2_a.j)*(w1_b.i - w1_a.i) -
    (w2_b.i - w2_a.i)*(w1_b.j - w1_a.j);

    // Parallel lines
    if(c == 0.0) {return -1;}

    float u = a/c;
    float v = b/c;

    if(0.0 <= u && u <= 1.0 && 0.0 <= v && v <= 1.0)
    {
      val->i = w1_a.i + u*(w1_b.i - w1_a.i);
      val->j = w1_a.j + u*(w1_b.j - w1_a.j);
      return 0;
    }

    return -1;
    */
}

float dot_vec4(Vec4 u, Vec4 v) {
    return u.i * v.i + u.j * v.j + u.k * v.k + u.l * v.l;
}

float dot_vec3(Vec3 u, Vec3 v) {
    return u.i * v.i + u.j * v.j + u.k * v.k;
}

Vec3 crosVec3(Vec3 u, Vec3 v) {
    Vec3 vec;
    vec.i = u.j * v.k - u.k * v.j;
    vec.j = u.k * v.i - u.i * v.k;
    vec.k = u.i * v.j - u.j * v.i;
    return vec;
}

Mat4 mat4(float a,
          float b,
          float c,
          float d,
          float e,
          float f,
          float g,
          float h,
          float i,
          float j,
          float k,
          float l,
          float m,
          float n,
          float o,
          float p) {
    Mat4 mat;
    mat.m[0] = a;
    mat.m[1] = b;
    mat.m[2] = c;
    mat.m[3] = d;
    mat.m[4] = e;
    mat.m[5] = f;
    mat.m[6] = g;
    mat.m[7] = h;
    mat.m[8] = i;
    mat.m[9] = j;
    mat.m[10] = k;
    mat.m[11] = l;
    mat.m[12] = m;
    mat.m[13] = n;
    mat.m[14] = o;
    mat.m[15] = p;
    return mat;
}

Mat3 mat3(float a,
          float b,
          float c,
          float d,
          float e,
          float f,
          float g,
          float h,
          float i) {
    Mat3 mat;
    mat.m[0] = a;
    mat.m[1] = b;
    mat.m[2] = c;
    mat.m[3] = d;
    mat.m[4] = e;
    mat.m[5] = f;
    mat.m[6] = g;
    mat.m[7] = h;
    mat.m[8] = i;
    return mat;
}

Mat4 perspective_matrix(float fov, float ratio, float near, float far) {
    float f =
        cos(fov / 2) / sin(fov / 2);  // cot(theta) = cos(theta)/sin(theta)
    return mat4(f / ratio,
                0,
                0,
                0,
                0,
                f,
                0,
                0,
                0,
                0,
                (far + near) / (near - far),
                2 * far * near / (near - far),
                0,
                0,
                -1,
                0);
}

Mat4 ortho(float l, float r, float b, float t, float n, float f) {
    float tx = -(r + l) / (r - l);
    float ty = -(t + b) / (t - b);
    float tz = -(f + n) / (f - n);

    Mat4 mat;
    mat.m[0] = 2 / (r - l);
    mat.m[1] = 0;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = 0;
    mat.m[5] = 2 / (t - b);
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = 0;
    mat.m[9] = 0;
    mat.m[10] = -2 / (f - n);
    mat.m[11] = 0;
    mat.m[12] = tx;
    mat.m[13] = ty;
    mat.m[14] = tz;
    mat.m[15] = 1;
    return mat;
}

Mat4 fps_view_rh(Vec3 eye, float pitch, float yaw) {
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);

    Vec3 xaxis = vec3(cosYaw, 0, -sinYaw);
    Vec3 yaxis = vec3(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
    Vec3 zaxis = vec3(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

    // Create a 4x4 view matrix from the right, up, forward and eye position
    // vectors
    return mat4(xaxis.i,
                xaxis.j,
                xaxis.k,
                -dot_vec3(xaxis, eye),
                yaxis.i,
                yaxis.j,
                yaxis.k,
                -dot_vec3(yaxis, eye),
                zaxis.i,
                zaxis.j,
                zaxis.k,
                -dot_vec3(zaxis, eye),
                0,
                0,
                0,
                1);
}

Mat4 multiply_mat4(Mat4 u, Mat4 v) {
    Mat4 mat;
    mat.m[0] =
        u.m[0] * v.m[0] + u.m[1] * v.m[4] + u.m[2] * v.m[8] + u.m[3] * v.m[12];
    mat.m[1] =
        u.m[0] * v.m[1] + u.m[1] * v.m[5] + u.m[2] * v.m[9] + u.m[3] * v.m[13];
    mat.m[2] =
        u.m[0] * v.m[2] + u.m[1] * v.m[6] + u.m[2] * v.m[10] + u.m[3] * v.m[14];
    mat.m[3] =
        u.m[0] * v.m[3] + u.m[1] * v.m[7] + u.m[2] * v.m[11] + u.m[3] * v.m[15];

    mat.m[4] =
        u.m[4] * v.m[0] + u.m[5] * v.m[4] + u.m[6] * v.m[8] + u.m[7] * v.m[12];
    mat.m[5] =
        u.m[4] * v.m[1] + u.m[5] * v.m[5] + u.m[6] * v.m[9] + u.m[7] * v.m[13];
    mat.m[6] =
        u.m[4] * v.m[2] + u.m[5] * v.m[6] + u.m[6] * v.m[10] + u.m[7] * v.m[14];
    mat.m[7] =
        u.m[4] * v.m[3] + u.m[5] * v.m[7] + u.m[6] * v.m[11] + u.m[7] * v.m[15];

    mat.m[8] = u.m[8] * v.m[0] + u.m[9] * v.m[4] + u.m[10] * v.m[8] +
               u.m[11] * v.m[12];
    mat.m[9] = u.m[8] * v.m[1] + u.m[9] * v.m[5] + u.m[10] * v.m[9] +
               u.m[11] * v.m[13];
    mat.m[10] = u.m[8] * v.m[2] + u.m[9] * v.m[6] + u.m[10] * v.m[10] +
                u.m[11] * v.m[14];
    mat.m[11] = u.m[8] * v.m[3] + u.m[9] * v.m[7] + u.m[10] * v.m[11] +
                u.m[11] * v.m[15];

    mat.m[12] = u.m[12] * v.m[0] + u.m[13] * v.m[4] + u.m[14] * v.m[8] +
                u.m[15] * v.m[12];
    mat.m[13] = u.m[12] * v.m[1] + u.m[13] * v.m[5] + u.m[14] * v.m[9] +
                u.m[15] * v.m[13];
    mat.m[14] = u.m[12] * v.m[2] + u.m[13] * v.m[6] + u.m[14] * v.m[10] +
                u.m[15] * v.m[14];
    mat.m[15] = u.m[12] * v.m[3] + u.m[13] * v.m[7] + u.m[14] * v.m[11] +
                u.m[15] * v.m[15];
    return mat;
}

Mat4 rotation_z_mat4(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat4 mat;
    mat.m[0] = c;
    mat.m[1] = -s;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = s;
    mat.m[5] = c;
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = 0;
    mat.m[9] = 0;
    mat.m[10] = 1;
    mat.m[11] = 0;
    mat.m[12] = 0;
    mat.m[13] = 0;
    mat.m[14] = 0;
    mat.m[15] = 1;
    return mat;
}

Mat4 rotation_y_mat4(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat4 mat;
    mat.m[0] = c;
    mat.m[1] = 0;
    mat.m[2] = s;
    mat.m[3] = 0;
    mat.m[4] = 0;
    mat.m[5] = 1;
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = -s;
    mat.m[9] = 0;
    mat.m[10] = c;
    mat.m[11] = 0;
    mat.m[12] = 0;
    mat.m[13] = 0;
    mat.m[14] = 0;
    mat.m[15] = 1;
    return mat;
}

Mat4 rotation_x_mat4(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat4 mat;
    mat.m[0] = 1;
    mat.m[1] = 0;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = 0;
    mat.m[5] = c;
    mat.m[6] = -s;
    mat.m[7] = 0;
    mat.m[8] = 0;
    mat.m[9] = s;
    mat.m[10] = c;
    mat.m[11] = 0;
    mat.m[12] = 0;
    mat.m[13] = 0;
    mat.m[14] = 0;
    mat.m[15] = 1;
    return mat;
}

Mat3 rotation_z_mat3(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat3 mat;
    mat.m[0] = c;
    mat.m[1] = -s;
    mat.m[2] = 0;
    mat.m[3] = s;
    mat.m[4] = c;
    mat.m[5] = 0;
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = 1;
    return mat;
}

Mat3 rotation_y_mat3(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat3 mat;
    mat.m[0] = c;
    mat.m[1] = 0;
    mat.m[2] = s;
    mat.m[3] = 0;
    mat.m[4] = 1;
    mat.m[5] = 0;
    mat.m[6] = -s;
    mat.m[7] = 0;
    mat.m[8] = c;
    return mat;
}

Mat3 rotation_x_mat3(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    Mat3 mat;
    mat.m[0] = 1;
    mat.m[1] = 0;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = c;
    mat.m[5] = -s;
    mat.m[6] = 0;
    mat.m[7] = s;
    mat.m[8] = c;
    return mat;
}

Mat4 translation_xyz_mat4(float x, float y, float z) {
    Mat4 mat;
    mat.m[0] = 1;
    mat.m[1] = 0;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = 0;
    mat.m[5] = 1;
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = 0;
    mat.m[9] = 0;
    mat.m[10] = 1;
    mat.m[11] = 0;
    mat.m[12] = x;
    mat.m[13] = y;
    mat.m[14] = z;
    mat.m[15] = 1;
    return mat;
}

Mat4 scale_xyz_mat4(float x, float y, float z) {
    Mat4 mat;
    mat.m[0] = x;
    mat.m[1] = 0;
    mat.m[2] = 0;
    mat.m[3] = 0;
    mat.m[4] = 0;
    mat.m[5] = y;
    mat.m[6] = 0;
    mat.m[7] = 0;
    mat.m[8] = 0;
    mat.m[9] = 0;
    mat.m[10] = z;
    mat.m[11] = 0;
    mat.m[12] = 0;
    mat.m[13] = 0;
    mat.m[14] = 0;
    mat.m[15] = 1;
    return mat;
}

void print_mat4(float *m) {
    return;
}

void print_mat3(float *m) {
    return;
}

/*
mat4 LookAtRH(vec3 eye, vec3 target, vec3 up)
{
    vec3 zaxis = normalize(eye - target);    // The "forward" vector.
    vec3 xaxis = normalize(cross(up, zaxis));// The "right" vector.
    vec3 yaxis = cross(zaxis, xaxis);     // The "up" vector.

    // Create a 4x4 view matrix from the right, up, forward and eye position
vectors mat4 viewMatrix = { vec4(     xaxis.x,          yaxis.x, zaxis.x, 0),
        vec4(     xaxis.y,          yaxis.y,          zaxis.y,     0),
        vec4(     xaxis.z,          yaxis.z,          zaxis.z,     0),
        vec4(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1)
    };

    return viewMatrix;
}
*/
