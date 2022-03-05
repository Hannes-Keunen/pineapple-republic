#version 450

layout (location = 0) in vec2 frag_uv;
layout (location = 1) in float frag_texid;

layout (location = 0) out vec4 color;

layout (binding = 0) uniform sampler2D u_textures[32];

void main() {
    color = texture(u_textures[int(frag_texid)], frag_uv);
}
