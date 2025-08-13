# version 460 core
layout(location = 0) in int i;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out float olightness;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
    vec3 Position;
};
struct Info {float lightness; float index; float verticalAngle; float horizontalAngle;};
layout(set=0, binding=1) uniform Origin {
    vec4 origin[32];
    Info infos[32];
    vec4 height[32];
};
layout(set=0, binding=2) uniform Corner {
    vec4 abovePlane[4];
};
layout(set=0, binding=10) uniform Lightness {
    float lightness_dark;
    float lightness_offset;
};
void main() {
    int mod = i%4, div = i/4;
    uv_out = vec2(0, infos[div].index*1.f/32);
    
    int nmod = (mod+int(infos[div].horizontalAngle))%4;

    vec4 pos = abovePlane[nmod] + origin[div];
    pos.z *= height[div][nmod];

    gl_Position = ClipPlane * pos;
    uv_out = vec2(abovePlane[mod].x, (abovePlane[mod].y + infos[div].index)*1.f/32);
    olightness = lightness_dark + max(lightness_offset, infos[div].lightness);
}