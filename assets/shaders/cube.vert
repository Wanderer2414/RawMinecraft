# version 460 core
layout(location = 0) in int i;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out float olightness;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
    vec3 Position;
};
struct Info {float lightness; float scale; float c; float type;};
layout(set=0, binding=1) uniform Origin {
    mat4 state[32];
    Info infos[32];
};
layout(set=0, binding=2) uniform Extra {
    vec4 extra;
};
layout(set=0, binding=3) uniform Corner {
    ivec3 cube_corner[36];
};
struct Vecs {vec2 a; vec2 b;};
layout(set=0, binding=4) uniform TexCoord {
    Vecs uv[18];
    vec2 eachSize;
};
layout(set=0, binding=10) uniform Lightness {
    float lightness_dark;
    float lightness_offset;
};
void main() {
    int mod = i%36, div = i/36;
    int index = int(infos[div].type);
    uv_out = vec2(index%3, index/3)*eachSize;
    if (infos[div].scale>0) {
        if (mod<6 || mod>=30) {
            if (Position.z<state[div][3].z) mod = 35-mod;
        } 
        else if (mod<12 || mod>=24) {
            if (Position.y<state[div][3].y) mod = 35-mod;
        } 
        else if (Position.x<state[div][3].x) mod = 35-mod;
        gl_Position = ClipPlane * state[div]*vec4(cube_corner[mod]*infos[div].scale + vec3(1)*(1-infos[div].scale)/2,1);
    }
    else gl_Position = ClipPlane * state[div]*vec4(cube_corner[mod],1);

    if (mod%2!=0) uv_out += uv[mod/2].b*extra.x;
    else uv_out += uv[mod/2].a*extra.x;
    uv_out.x /= extra.z;
    uv_out.y /= extra.w;
    olightness = lightness_dark + max(lightness_offset, infos[div].lightness);
}