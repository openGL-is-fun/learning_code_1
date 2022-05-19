#version 430

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;

layout(location = 0) in vec3 vert_in;
layout(location = 1) in vec3 color_in;

out vec4 color_vert;

void main() {
    color_vert = vec4(color_in, 1.0);
    gl_Position = proj * view * model * vec4(vert_in, 1.0);
}