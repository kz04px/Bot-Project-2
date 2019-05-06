#version 430

uniform mat4 vp_matrix;

in vec2 vertex;
in vec2 position;
in vec3 col;
in float rotation;

out vec3 colour;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
  gl_Position = vp_matrix * (rotationMatrix(vec3(0,0,1), rotation) * vec4(vertex, 0.0, 1.0) + vec4(position, 0.0, 0.0));
  //gl_Position = vp_matrix * rotationMatrix(vec3(0,0,1), rotation) * vec4(vertex + position, 0.0, 1.0);
  colour = col; //vec3(rotation, 0.0, 0.0);
}
