#version 330

layout(location=0) in vec3 a_Position;

uniform mat4 u_PVM;
uniform mat4 u_Model;

out vec3 v_WordlPos;

void main() {
	gl_Position = u_PVM * vec4(a_Position, 1.0);

	v_WordlPos = (u_Model * vec4(a_Position, 1.0)).xyz;
}