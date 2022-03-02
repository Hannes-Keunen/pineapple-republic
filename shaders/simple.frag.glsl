#version 450

layout (location = 0) in vec2 frag_uv;

layout (location = 0) out vec4 color;

layout (binding = 0) uniform sampler2D u_texture;

void main() {
    // color = texture(u_texture, frag_uv);
    color = vec4(0.8, 0.2, 0.2, 1.0);
}
