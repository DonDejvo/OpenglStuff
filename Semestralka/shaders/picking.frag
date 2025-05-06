#version 330

uniform uint u_ObjectID;

out uvec2 FragColor;

void main() {
	FragColor = uvec2(u_ObjectID, gl_PrimitiveID);
}