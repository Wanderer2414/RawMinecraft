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
layout(set=0, binding=2) uniform State {
    mat4 state[32];
};
layout(set=0, binding=3) uniform Extra {
    float img;
    float scale;
    vec2 size;
};
layout(set=0, binding=4) uniform Corner {
    ivec3 cube_corner[36];
};
struct Vecs {vec2 a; vec2 b;};
layout(set=0, binding=5) uniform TexCoord {
    Vecs uv[18];
    vec2 eachSize;
};
void main() {
    int mod = i%36, div = i/36;
    int index = int(info[div].index);
    uv_out = vec2(index%3, index/3)*eachSize;
    
    vec4 pos = vec4(cube_corner[mod],1);

    pos*=state[div];

    pos += vec4(info[div].position, 0);

    if (mod%2!=0) uv_out += uv[mod/2].b*img;
    else uv_out += uv[mod/2].a*img;
    uv_out /= size;
    gl_Position = ClipPlane * pos;
}