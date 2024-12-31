#version 330 core

out vec4 frag_color;

in vec3 v_vertex_position;
in vec2 v_uv;

uniform sampler2D s_tex;

void main() {
    frag_color = vec4(texture(s_tex, v_uv).rgb, 1.0);
}