#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 0) out vec2 uv_out;
layout(set=0,binding=2) uniform Origin {
    vec2 origin;
};
  
layout(set=0,binding=3) uniform Scale {
    vec2 scale;
};
void main() {
    gl_Position = vec4(origin + position*scale, 0, 1.0);
    uv_out = uv;
}