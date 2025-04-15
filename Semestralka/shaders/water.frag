#version 330

in vec4 v_ClipSpace;

uniform sampler2D u_TextureReflection;
uniform sampler2D u_TextureRefraction;

out vec4 FragColor;

void main() {
	vec2 ndc = (v_ClipSpace.xy / v_ClipSpace.w) * 0.5 + vec2(0.5);

	vec4 reflectionColor = texture(u_TextureReflection, vec2(ndc.x, 1.0 - ndc.y));
	vec4 refractionColor = texture(u_TextureRefraction, ndc);

	FragColor = mix(reflectionColor, refractionColor, 0.5);
}