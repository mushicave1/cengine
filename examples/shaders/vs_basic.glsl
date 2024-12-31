#version 330 core

layout (location = 0) in vec3 a_position;

out vec3 v_vertex_position;

void main() {
    v_vertex_position = a_position;
    gl_Position = vec4(a_position, 1.0);
}