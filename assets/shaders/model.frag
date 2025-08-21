# version 460 core
layout(location = 0) in vec2 uv;

layout(binding=0) uniform sampler2D tex;
layout(location = 0) out vec4 frag_color;

layout(set=0, binding=2) uniform Color {
    vec4 color;
};

vec4 alphaBlend(vec4 fg, vec4 bg) {
    float outAlpha = fg.a + bg.a * (1.0 - fg.a);

    // Avoid divide-by-zero when resulting alpha is 0
    if (outAlpha < 0.0001)
        return vec4(0.0);

    vec3 outRGB = (fg.rgb * fg.a + bg.rgb * bg.a * (1.0 - fg.a)) / outAlpha;
    
    return vec4(outRGB, outAlpha);
};

void main() {
    frag_color = alphaBlend(color, texture(tex, uv));
}