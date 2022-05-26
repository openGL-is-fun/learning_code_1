#version 430

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;

layout(location = 0) in vec3 vert_in;
layout(location = 1) in vec4 color_in;

out vec4 color_vert;

// color
// depth
// are you behind something already drawn? => yes => skip it

void main() {
    color_vert = color_in;
    gl_Position = proj * view * model * vec4(vert_in, 1.0);
}