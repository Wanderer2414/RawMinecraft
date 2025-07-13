#version 460 core
layout(location = 0) in vec2 position;
layout(set=0, binding=1) uniform Origin {
    vec2 origin;
};
void main() {
    gl_Position = vec4(origin+position, 0, 1.0);
}