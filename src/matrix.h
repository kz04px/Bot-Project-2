#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

/*
 We use radians here

 [ 0] [ 1] [ 2] [ 3]
 [ 4] [ 5] [ 6] [ 7]
 [ 8] [ 9] [10] [11]
 [12] [13] [14] [15]
*/

typedef struct
{
  float m[16];
} s_mat4;

typedef struct
{
  float m[9];
} s_mat3;

typedef struct
{
  float i;
  float j;
  float k;
  float l;
} s_vec4;

typedef struct
{
  float i;
  float j;
  float k;
} s_vec3;

typedef struct
{
  float i;
  float j;
} s_vec2;

float dot_vec3(s_vec3 u, s_vec3 v);
float dot_vec4(s_vec4 u, s_vec4 v);
s_vec3 cross_vec3(s_vec3 u, s_vec3 v);
s_mat4 multiply_mat4(s_mat4 u, s_mat4 v);
void print_mat4(float *m);
void print_mat3(float *m);

s_vec2 vec2(float i, float j);
s_vec3 vec3(float i, float j, float k);
s_vec4 vec4(float i, float j, float k, float l);
s_mat3 mat3(float, float, float, float, float, float, float, float, float);
s_mat4 mat4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

s_mat4 identity_matrix();
s_mat4 perspective_matrix(float fov, float ratio, float near, float far);
s_mat4 fps_view_rh(s_vec3 eye, float pitch, float yaw);
s_mat3 rotation_z_mat3(float angle);
s_mat3 rotation_y_mat3(float angle);
s_mat3 rotation_x_mat3(float angle);
s_mat4 rotation_z_mat4(float angle);
s_mat4 rotation_y_mat4(float angle);
s_mat4 rotation_x_mat4(float angle);
s_mat4 translation_xyz_mat4(float x, float y, float z);
s_mat4 ortho(float l, float r, float b, float t, float n, float f);
s_mat4 scale_xyz_mat4(float x, float y, float z);

int line_line_intersection(s_vec2* val, s_vec2 w1_a, s_vec2 w1_b, s_vec2 w2_a, s_vec2 w2_b);

/*
void identity_matrix(float *m);
void translation_matrix(float *m, float x, float y, float z);
void print_matrix(float *m);
void transpose_matrix(float *m);
void rotation_matrix(float *m, float angle);
void rotate_matrix(float *m, float angle);
void translate_matrix(float *m, float x, float y, float z);
*/

#endif // MATRIX_H_INCLUDED
