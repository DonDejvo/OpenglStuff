#version 330

layout(location=0) in vec3 a_Position;

uniform mat4 u_PVM;

out vec4 v_ClipSpace;

void main() {

    v_ClipSpace = u_PVM * vec4(a_Position, 1.0);

    gl_Position = v_ClipSpace;
}