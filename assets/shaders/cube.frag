# version 460 core
layout(location = 0) in vec2 uv;

layout(binding=0) uniform sampler2D tex;
layout(location = 0) out vec4 frag_color;
void main() {
    frag_color = texture(tex, uv);
}