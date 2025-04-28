#version 330

in vec2 v_TexCoord;

uniform sampler2D u_DiffuseTexture;

out vec4 FragColor;

void main() {
	FragColor = texture(u_DiffuseTexture, v_TexCoord);
}