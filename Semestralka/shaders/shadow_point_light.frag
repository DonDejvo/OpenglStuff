#version 330

in vec3 v_WorldPos;

uniform vec3 u_LightPosition;

out float lightToPixelDist;

void main() {
	vec3 lightToPixel = v_WorldPos - u_LightPosition;
 
	lightToPixelDist = length(lightToPixel);
}