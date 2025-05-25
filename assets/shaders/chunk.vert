# version 460 core
layout(location = 0) in vec3 cube_corner;

layout(location = 0) out vec4 cube_color;

layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    vec4 origin_point;
    vec4 colors;
};
void main() {
    cube_color = colors;
    gl_Position = ClipPlane * vec4(vec3(origin_point)+cube_corner, 1.0);
}