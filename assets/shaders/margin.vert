# version 460 core
layout(location = 0) in vec3 cube_corner;
layout(location = 0) out vec3 color;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    vec3 origin_point;
};
layout(set=0, binding=2) uniform Margin {
    vec3 margin;
    float padding;
    vec3 colors;
};
void main() {
    color = colors;
    gl_Position = ClipPlane * vec4(origin_point+cube_corner*margin, 1.0);
}