# version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    mat4 origin;
};
void main() {
    gl_Position = ClipPlane * origin * vec4(position.x, position.z, position.y, 1.0);
    uv_out = uv;
}