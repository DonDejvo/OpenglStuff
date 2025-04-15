#version 330

struct Fog {
    vec3 Color;
    float Density;
    float Gradient;
};

in vec2 v_TexCoord;
in vec3 v_WorldPosition;
in vec4 v_ClipSpace;

uniform bool u_DistortionEnabled = false;

uniform float u_DistortionIntensity;

uniform vec3 u_CameraPosition;

uniform sampler2D u_TextureReflection;
uniform sampler2D u_TextureRefraction;
uniform sampler2D u_DistortionMap;

uniform float u_Time;
const float moveSpeed = 0.06;

uniform bool u_FogEnabled = false;
uniform Fog u_Fog;

out vec4 FragColor;

float CalcFogFactor() {
    float distFromCameraToPixel = distance(v_WorldPosition, u_CameraPosition);
    float factor = exp(-pow(distFromCameraToPixel * u_Fog.Density, u_Fog.Gradient));
    return clamp(factor, 0.0, 1.0);
}

void main() {
	vec2 ndc = (v_ClipSpace.xy / v_ClipSpace.w) * 0.5 + vec2(0.5);
	vec2 reflectionCoord = vec2(ndc.x, 1.0 - ndc.y);
	vec2 refractionCoord = vec2(ndc.x, ndc.y);

	if(u_DistortionEnabled) {
		vec2 distortion = (texture(u_DistortionMap, vec2(v_TexCoord.x + u_Time * moveSpeed, v_TexCoord.y)).xy * 2.0 - vec2(1.0)) * u_DistortionIntensity;

		reflectionCoord += distortion;
		refractionCoord += distortion;
	}

	vec4 reflectionColor = texture(u_TextureReflection, reflectionCoord);
	vec4 refractionColor = texture(u_TextureRefraction, refractionCoord);

	FragColor = mix(reflectionColor, refractionColor, 0.5);
	if(u_FogEnabled) {
        float fogFactor = CalcFogFactor();
        FragColor = mix(vec4(u_Fog.Color, 1.0), FragColor, fogFactor);
    }
}