#version 330

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;

layout(location=2) in mat4 a_ViewModel;
layout(location=6) in vec4 a_SpriteRegion;

uniform mat4 u_Proj;

//uniform mat4 u_ViewModel;
//uniform ivec2 u_SpriteIndex;

uniform ivec2 u_SpritesheetInfo; // (columns, number of sprites)

out vec2 v_TexCoord;

void main() {
	gl_Position = u_Proj * a_ViewModel * vec4(a_Position, 1.0);

	v_TexCoord = a_SpriteRegion.xy + a_SpriteRegion.zw * a_TexCoord;
}