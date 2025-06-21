# version 460 core
layout(location = 0) in float index;

layout(location = 0) out vec2 uv_out;
layout(set=0, binding=0) uniform CameraBuffer{
    mat4 ClipPlane;
};
layout(set=0, binding=1) uniform Origin {
    vec4 origin_point[32];
};
layout(set=0, binding=2) uniform Corner {
    vec4 cube_corner[14];
};
layout(set=0, binding=3) uniform TexCoord {
    vec2 uv[14];
};
void main() {
    int i = int(index);
    uv_out = uv[i%14];
    gl_Position = ClipPlane * (origin_point[i/14] + cube_corner[i%14]);
}