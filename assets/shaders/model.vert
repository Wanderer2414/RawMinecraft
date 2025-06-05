# version 460 core
layout(location = 0) in vec3 position;

layout(location = 0) out vec3 color;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    mat4 offset;
};
void main() {
    gl_Position = ClipPlane * offset * vec4(position.x, position.z, position.y, 1.0);
    color = vec3(1, 1, 1);
}