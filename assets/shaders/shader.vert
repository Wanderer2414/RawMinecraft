#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 vColor;

layout(set = 0, binding = 0) uniform Matrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    vColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
