#version 460 core
layout(location = 0) out vec4 outColor;
layout(set=0,binding=2) uniform Color {
    vec4 color;
};
void main() {
    outColor = color;
}