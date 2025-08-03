# version 460 core
layout(location = 0) in int i;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
    vec3 Position;
};
struct Info { ivec3 position; int index; };
layout(set=0, binding=1) uniform Origin {
    Info info[32];
};
layout(set=0, binding=2) uniform Corner {
    ivec3 cube_corner[36];
};
struct Vecs {vec2 a; vec2 b;};
layout(set=0, binding=3) uniform TexCoord {
    Vecs uv[18];
    vec2 rate;
};
void main() {
    int mod = i%36, div = i/36;
    uv_out = vec2((info[div].index%3)*rate.x, (info[div].index/3)*rate.y);

    if (mod<6 || mod>=30) {
        if (Position.z<info[div].position.z) mod = 35-mod;
    } 
    else if (mod<12 || mod>=24) {
        if (Position.y<info[div].position.y) mod = 35-mod;
    } 
    else if (Position.x<info[div].position.x) mod = 35-mod;
    
    ivec3 pos = info[div].position + cube_corner[mod];
    
    if (mod%2!=0) uv_out += uv[mod/2].b;
    else uv_out += uv[mod/2].a;
    gl_Position = ClipPlane * vec4(pos,1);
}