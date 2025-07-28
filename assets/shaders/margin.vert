# version 460 core
layout(location = 0) in int i;
layout(location = 0) out vec4 color;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
struct Info {ivec3 position; int scale;};
layout(set=0, binding=1) uniform Origin {
    Info infos[32];
};
layout(set=0, binding=2) uniform Fragment {
    vec4 colors;
};
layout(set=0, binding=3) uniform CubeCorner {
    ivec3 cube_corner[18];
};
void main() {
    gl_Position = ClipPlane * vec4(cube_corner[i%18]*infos[i/18].scale + infos[i/18].position, 1);
    color = colors;
}