# version 460 core
layout(location = 0) in vec3 cube_corner;
layout(location = 1) in vec3 origin_point;

layout(location = 0) out vec3 color;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
void main() {
    color = vec3(1, 1, 1);
    gl_Position = ClipPlane * vec4(origin_point+cube_corner, 1.0);
}