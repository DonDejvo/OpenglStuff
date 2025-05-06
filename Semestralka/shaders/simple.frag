#version 330

in vec2 v_TexCoord;

uniform sampler2D u_TextureDiffuse;
uniform vec3 u_DiffuseColor;
uniform bool u_DiffuseEnabled;

out vec4 FragColor;

void main() {
    FragColor = vec4(u_DiffuseColor, 1.0);
    if(u_DiffuseEnabled) {
        FragColor *= texture(u_TextureDiffuse, v_TexCoord);
    }
}