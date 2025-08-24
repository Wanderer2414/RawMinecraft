# version 460 core
layout(location = 0) in vec2 uv;
layout(location = 1) in float lightness;
layout(binding=0) uniform sampler2D tex;
layout(location = 0) out vec4 frag_color;

layout(set=0, binding=10) uniform Lightness {
    vec4 view_color;
    float lightness_dark;
    float lightness_offset;
};

vec3 hsl2rgb(vec3 hsl) {
    float h = hsl.x;
    float s = hsl.y;
    float l = hsl.z;

    float c = (1.0 - abs(2.0 * l - 1.0)) * s;
    float h_ = h * 6.0;
    float x = c * (1.0 - abs(mod(h_, 2.0) - 1.0));

    vec3 rgb;

    if (0.0 <= h_ && h_ < 1.0) {
        rgb = vec3(c, x, 0.0);
    } else if (1.0 <= h_ && h_ < 2.0) {
        rgb = vec3(x, c, 0.0);
    } else if (2.0 <= h_ && h_ < 3.0) {
        rgb = vec3(0.0, c, x);
    } else if (3.0 <= h_ && h_ < 4.0) {
        rgb = vec3(0.0, x, c);
    } else if (4.0 <= h_ && h_ < 5.0) {
        rgb = vec3(x, 0.0, c);
    } else if (5.0 <= h_ && h_ < 6.0) {
        rgb = vec3(c, 0.0, x);
    } else {
        rgb = vec3(0.0, 0.0, 0.0);
    }

    float m = l - 0.5 * c;
    return rgb + vec3(m);
}

vec3 rgb2hsl(vec3 rgb) {
    float r = rgb.r;
    float g = rgb.g;
    float b = rgb.b;

    float maxVal = max(max(r, g), b);
    float minVal = min(min(r, g), b);
    float delta = maxVal - minVal;

    float h = 0.0;
    float s = 0.0;
    float l = (maxVal + minVal) * 0.5;

    if (delta != 0.0) {
        s = (l < 0.5) ? (delta / (maxVal + minVal)) : (delta / (2.0 - maxVal - minVal));

        if (maxVal == r) {
            h = (g - b) / delta + (g < b ? 6.0 : 0.0);
        } else if (maxVal == g) {
            h = (b - r) / delta + 2.0;
        } else {
            h = (r - g) / delta + 4.0;
        }

        h /= 6.0;
    }

    return vec3(h, s, l);
}
vec4 alphaBlend(vec4 fg, vec4 bg) {
    float outAlpha = fg.a + bg.a * (1.0 - fg.a);

    // Avoid divide-by-zero when resulting alpha is 0
    if (outAlpha < 0.0001)
        return vec4(0.0);

    vec3 outRGB = (fg.rgb * fg.a + bg.rgb * bg.a * (1.0 - fg.a)) / outAlpha;
    
    return vec4(outRGB, outAlpha);
}

void main() {
    frag_color = texture(tex, uv);
    
    vec3 hsl = rgb2hsl(vec3(frag_color));

    if (hsl.z + lightness < 0) hsl.z = 0;
    else if (lightness + hsl.z < 1) hsl.z += lightness;
    else hsl.z = 1;

    frag_color = alphaBlend(view_color, vec4(hsl2rgb(hsl), frag_color.w));
}