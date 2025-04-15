#version 330

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;

uniform mat4 u_PVM;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec3 v_WorldPosition;
out vec4 v_ClipSpace;

void main() {
    vec4 clipSpace = u_PVM * vec4(a_Position, 1.0);
    vec4 worldPosition = u_Model * vec4(a_Position, 1.0);

    v_TexCoord = a_TexCoord * 6.0;
    v_WorldPosition = worldPosition.xyz;
    v_ClipSpace = clipSpace;

    gl_Position = clipSpace;
}