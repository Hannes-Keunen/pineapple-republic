#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 frag_uv;

layout (location = 0) uniform mat4 vpmatrix;

void main() {
    frag_uv = uv;
    gl_Position = vpmatrix * vec4(position, 0.0, 1.0);
}
