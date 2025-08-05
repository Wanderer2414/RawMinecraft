# version 460 core
layout(location = 0) in int i;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
    vec3 Position;
};
struct Info { vec3 position; float index; };
layout(set=0, binding=1) uniform Origin {
    Info info[32];
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
void main() {
    int mod = i%36, div = i/36;
    int index = int(info[div].index);
    uv_out = vec2(index%3, index/3)*eachSize;
    if (mod<6 || mod>=30) {
        if (Position.z<info[div].position.z) mod = 35-mod;
    } 
    else if (mod<12 || mod>=24) {
        if (Position.y<info[div].position.y) mod = 35-mod;
    } 
    else if (Position.x<info[div].position.x) mod = 35-mod;
    
    vec4 pos = vec4(info[div].position + extra.y*cube_corner[mod],1);
    if (mod%2!=0) uv_out += uv[mod/2].b*extra.x;
    else uv_out += uv[mod/2].a*extra.x;
    uv_out.x /= extra.z;
    uv_out.y /= extra.w;
    gl_Position = ClipPlane * pos;
}