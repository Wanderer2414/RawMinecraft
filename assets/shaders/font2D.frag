# version 460 core
layout(location = 0) in vec2 uv;

layout(binding=0) uniform sampler2D tex;
layout(location = 0) out vec4 frag_color;
layout(set=0, binding=1) uniform TextInfo {
    vec3 color;
};
void main() {
    float alpha = texture(tex,uv).r;
    frag_color = vec4(color, alpha);
}