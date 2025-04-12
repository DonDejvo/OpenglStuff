#version 330

layout(location=0) in vec3 a_Position;

uniform mat4 u_PV;

out vec3 v_TexCoord;

void main() {
	v_TexCoord = a_Position;

	vec4 PVPosition = u_PV * vec4(a_Position, 1.0);
	gl_Position = PVPosition.xyww;
}