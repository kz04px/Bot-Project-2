#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

/*
 We use radians here

 [ 0] [ 1] [ 2] [ 3]
 [ 4] [ 5] [ 6] [ 7]
 [ 8] [ 9] [10] [11]
 [12] [13] [14] [15]
*/

struct Mat4 {
    float m[16];
};

struct Mat3 {
    float m[9];
};

struct Vec4 {
    float i;
    float j;
    float k;
    float l;
};

struct Vec3 {
    float i;
    float j;
    float k;
};

struct Vec2 {
    float i;
    float j;
};

float dot_vec3(Vec3 u, Vec3 v);
float dot_vec4(Vec4 u, Vec4 v);
Vec3 crosVec3(Vec3 u, Vec3 v);
Mat4 multiply_mat4(Mat4 u, Mat4 v);
void print_mat4(float *m);
void print_mat3(float *m);

Vec2 vec2(float i, float j);
Vec3 vec3(float i, float j, float k);
Vec4 vec4(float i, float j, float k, float l);
Mat3 mat3(float, float, float, float, float, float, float, float, float);
Mat4 mat4(float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float,
          float);

Mat4 identity_matrix();
Mat4 perspective_matrix(float fov, float ratio, float near, float far);
Mat4 fps_view_rh(Vec3 eye, float pitch, float yaw);
Mat3 rotation_z_mat3(float angle);
Mat3 rotation_y_mat3(float angle);
Mat3 rotation_x_mat3(float angle);
Mat4 rotation_z_mat4(float angle);
Mat4 rotation_y_mat4(float angle);
Mat4 rotation_x_mat4(float angle);
Mat4 translation_xyz_mat4(float x, float y, float z);
Mat4 ortho(float l, float r, float b, float t, float n, float f);
Mat4 scale_xyz_mat4(float x, float y, float z);

int line_line_intersection(Vec2 *val,
                           Vec2 w1_a,
                           Vec2 w1_b,
                           Vec2 w2_a,
                           Vec2 w2_b);

/*
void identity_matrix(float *m);
void translation_matrix(float *m, float x, float y, float z);
void print_matrix(float *m);
void transpose_matrix(float *m);
void rotation_matrix(float *m, float angle);
void rotate_matrix(float *m, float angle);
void translate_matrix(float *m, float x, float y, float z);
*/

#endif  // MATRIX_H_INCLUDED
