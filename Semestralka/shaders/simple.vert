#version 330

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;
layout(location=2) in vec3 a_Normal;

uniform mat4 u_PVM;
uniform mat4 u_Model;

out vec2 texCoord;

void main() {
    texCoord = a_TexCoord;

    gl_Position = u_PVM * vec4(a_Position, 1.0);
}