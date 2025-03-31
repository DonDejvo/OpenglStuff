#version 330

struct Material {
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
};

struct BaseLight {
    vec3 Color;
    float AmbietIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};

in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldPosition;

uniform sampler2D u_TextureDiffuse;
uniform sampler2D u_TextureSpecular;
uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;

out vec4 color;

vec4 CalcLightInternal(BaseLight base, vec3 direction, vec3 normal) {
    vec4 ambientColor = vec4(base.Color, 1.0) * base.AmbietIntensity * vec4(u_Material.AmbientColor, 1.0);

    vec4 diffuseColor = vec4(0.0);

    float diffuseFactor = dot(normal, -direction);

    if(diffuseFactor > 0.0) {
        diffuseColor = vec4(base.Color, 1.0) * base.DiffuseIntensity * vec4(u_Material.DiffuseColor, 1.0) * diffuseFactor;
    }

    return ambientColor + diffuseColor;
}

vec4 CalcDirectionalLight(vec3 normal) {
    return CalcLightInternal(u_DirectionalLight.Base, u_DirectionalLight.Direction, normal);
}

vec4 CalcPhongLighting() {
    vec3 normal = normalize(worldNormal);

    vec4 totalLighting = CalcDirectionalLight(normal);

    vec4 finalColor = texture(u_TextureDiffuse, texCoord) * totalLighting;

    return finalColor;
}

void main() {
    color = CalcPhongLighting();
}