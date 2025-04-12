#version 330

in vec3 v_TexCoord;

uniform samplerCube u_TextureCubemap;

out vec4 FragColor;

void main() {
	FragColor = texture(u_TextureCubemap, v_TexCoord);
}