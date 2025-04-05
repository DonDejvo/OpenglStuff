#version 330

in vec2 v_TexCoord;

uniform sampler2D u_TextureDiffuse;

out vec4 color;

void main() {
    color = texture(u_TextureDiffuse, v_TexCoord);
}