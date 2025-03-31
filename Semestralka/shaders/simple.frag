#version 330

in vec2 texCoord;

uniform sampler2D u_TextureDiffuse;

out vec4 color;

void main() {
    color = texture(u_TextureDiffuse, texCoord);
}