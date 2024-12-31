#version 330 core

out vec4 frag_color;

in vec3 v_vertex_position;

void main() {
    frag_color = vec4(v_vertex_position, 1.0);
}