#version 430

in vec4 color_vert;
in vec3 normal_in;
in vec3 fragPos;

out vec4 color_out;

layout(location = 3) uniform vec4 lightColor;
layout(location = 4) uniform vec3 lightPos;

void main() {
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal_in);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec4 result = (ambient + diffuse) * color_vert;
    color_out = result;
}
