# version 460 core
layout(location = 0) in int i;
layout(location = 0) out vec3 color;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    vec4 position[32];
};
layout(set=0, binding=2) uniform CubeCorner {
    vec4 cube_corner[14];
};
void main() {
    gl_Position = ClipPlane * (cube_corner[i%14]*16 + position[i/14]);
    color = vec3(1,0,0);
}