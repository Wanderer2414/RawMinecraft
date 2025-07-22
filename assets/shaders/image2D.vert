#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 iUv;
layout(location = 0) out vec2 oUv;

layout(set = 0, binding = 0) uniform OriginBuffer {
    vec2 origin;
};
layout(set = 0, binding = 1) uniform OriginSizeBuffer {
    vec2 size;
};
layout(set = 0, binding = 2) uniform TexturePosBuffer {
    vec2 iposition;
};

layout(set = 0, binding = 3) uniform TextureSizeBuffer {
    vec2 isize;
};

void main() {
    gl_Position = vec4(origin + pos*size, 0, 1.0);
    oUv = iposition + iUv*isize;
}
