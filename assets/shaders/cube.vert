# version 460 core
layout(location = 0) in int i;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
struct Info { ivec3 position; int index; };
layout(set=0, binding=1) uniform Origin {
    Info info[32];
};
layout(set=0, binding=2) uniform Corner {
    ivec3 cube_corner[18];
};
struct Vecs {vec2 a; vec2 b;};
layout(set=0, binding=3) uniform TexCoord {
    Vecs uv[9];
    vec2 rate;
};
void main() {
    ivec3 pos = info[i/18].position + cube_corner[i%18];

    int index = info[i/18].index;

    uv_out = vec2((index%3)*rate.x, (index/3)*rate.y);
    index = (i%18)/2;
    if (i%2!=0) uv_out += uv[index].b;
    else uv_out += uv[index].a;
    gl_Position = ClipPlane * vec4(pos,1);
}