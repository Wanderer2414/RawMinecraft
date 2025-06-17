#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 0) out vec3 vColor;
layout(set = 0, binding = 0) uniform Matrices {
    mat4 clipPlane;
};
layout(set = 0, binding = 1) uniform Info {
    vec3 color;    
};

void main() {
    vColor = color;
    gl_Position = clipPlane * vec4(aPos, 1.0);
}
