# version 460 core
layout(location = 0) in vec3 cube_corner;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    vec3 origin_point;
};
void main() {
    uv_out = uv;
    gl_Position = ClipPlane * vec4(origin_point+cube_corner, 1.0);
}