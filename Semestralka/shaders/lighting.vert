#version 330

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;
layout(location=2) in vec3 a_Normal;

uniform mat4 u_PVM;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPosition;

void main() {
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(u_Model) * a_Normal;
    v_WorldPosition = (u_Model * vec4(a_Position, 1.0)).xyz;

    gl_Position = u_PVM * vec4(a_Position, 1.0);
}