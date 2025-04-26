#version 330

const int MAX_POINT_LIGHTS = 16;
const int MAX_SPOT_LIGHTS = 16;

struct Material {
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    float Shininess;
    float Alpha;
    bool BidirectionalNormals;
};

struct BaseLight {
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation {
    float Const;
    float Linear;
    float Exp;
};

struct PointLight {
    BaseLight Base;
    vec3 Position;
    Attenuation Attenuation;
};

struct SpotLight {
    PointLight Base;
    vec3 Direction;
    float CutOff;
};

struct Fog {
    vec3 Color;
    float Density;
    float Gradient;
};

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPosition;
in vec4 v_LightSpacePosition;
in vec3 v_Tangent;
in vec3 v_Bitangent;

uniform bool u_SpecularEnabled = false;
uniform bool u_DiffuseTextureEnabled = false;
uniform bool u_NormalMapEnabled = false;

uniform sampler2D u_TextureDiffuse;
uniform sampler2D u_TextureSpecular;
uniform sampler2D u_ShadowMap;
uniform sampler2D u_NormalMap;

uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;
uniform vec3 u_CameraPosition;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
uniform int u_NumPointLights;
uniform int u_NumSpotLights;

uniform bool u_FogEnabled = false;
uniform Fog u_Fog;

out vec4 FragColor;

float CalcFogFactor() {
    float distFromCameraToPixel = distance(v_WorldPosition, u_CameraPosition);
    float factor = exp(-pow(distFromCameraToPixel * u_Fog.Density, u_Fog.Gradient));
    return clamp(factor, 0.0, 1.0);
}

vec3 CalcShadowCoords() {
    vec3 projCoords = v_LightSpacePosition.xyz / v_LightSpacePosition.w;
    vec3 shadowCoords = projCoords * 0.5 + vec3(0.5);
    return shadowCoords;
}

float CalcShadowFactor()
{
    vec3 shadowCoords = CalcShadowCoords();
    if(shadowCoords.x < 0.0 || shadowCoords.x > 1.0 || shadowCoords.y < 0.0 || shadowCoords.y > 1.0) {
        return 1.0;
    }

    float depth = texture(u_ShadowMap, shadowCoords.xy).r;
    float bias = 0.005;

    if(shadowCoords.z - bias > depth) {
        return 0.5;
    }
    return 1.0;
}

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal, float shadowFactor) {
    vec4 ambientColor = vec4(light.Color, 1.0) * light.AmbientIntensity * vec4(u_Material.AmbientColor, 1.0);

    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normal, -lightDirection);
    if(u_Material.BidirectionalNormals) {
        diffuseFactor = abs(diffuseFactor);
    }

    if(diffuseFactor > 0.0) {
        diffuseColor = vec4(light.Color, 1.0) * light.DiffuseIntensity * vec4(u_Material.DiffuseColor, 1.0) * diffuseFactor;

        vec3 pixelToCamera = normalize(u_CameraPosition - v_WorldPosition);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));

        float specularFactor = dot(pixelToCamera, lightReflect);

        if(specularFactor > 0.0) {
            float specularExponent = u_Material.Shininess * 255.0;

            if(u_SpecularEnabled) {
                specularExponent = texture(u_TextureSpecular, v_TexCoord).r * 255.0;
            }

            specularFactor = pow(specularFactor, specularExponent);

            specularColor = vec4(light.Color, 1.0) * light.DiffuseIntensity * vec4(u_Material.SpecularColor, 1.0) * specularFactor;
        }
    }

    return ambientColor + (diffuseColor + specularColor) * shadowFactor;
}

vec4 CalcDirectionalLight(vec3 normal) {
    float shadowFactor = CalcShadowFactor();
    return CalcLightInternal(u_DirectionalLight.Base, u_DirectionalLight.Direction, normal, shadowFactor);
}

vec4 CalcPointLight(PointLight light, vec3 normal) {
    vec3 direction = v_WorldPosition - light.Position;
    float dist = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcShadowFactor();
    vec4 color = CalcLightInternal(light.Base, direction, normal, shadowFactor);

    return color / (light.Attenuation.Const + dist * (light.Attenuation.Linear + dist * light.Attenuation.Exp));
}

vec4 CalcSpotLight(SpotLight light, vec3 normal) {
    vec3 lightToPixel = normalize(v_WorldPosition - light.Base.Position);
    float spotFactor = dot(lightToPixel, light.Direction);

    if(spotFactor > light.CutOff) { // angle < cutoffAngle -> cos(angle) > cos(cutoffAngle)
        float lightIntensity = 1 - (1 - spotFactor) / (1 - light.CutOff); // 1 - angle / cutoffAngle -> 1 - (1 - cos(angle)) / (1 - cos(cutoffAngle))
        return CalcPointLight(light.Base, normal) * lightIntensity;
    }
    return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 CalcPhongLighting() {
    vec3 normal = normalize(v_Normal);
    if(u_NormalMapEnabled) {
        vec3 bumpMapNormal = texture(u_NormalMap, v_TexCoord).rgb;
        bumpMapNormal = bumpMapNormal * 2.0 - vec3(1.0);

        vec3 tangent = normalize(v_Tangent);
        vec3 bitangent = normalize(v_Bitangent);
        mat3 TBN = mat3(tangent, bitangent, normal);

        normal = normalize(TBN * bumpMapNormal);
    }

    vec4 totalLighting = CalcDirectionalLight(normal);

    for(int i = 0; i < u_NumPointLights; ++i) {
        totalLighting += CalcPointLight(u_PointLights[i], normal);
    }

    for(int i = 0; i < u_NumSpotLights; ++i) {
        totalLighting += CalcSpotLight(u_SpotLights[i], normal);
    }

    vec4 finalColor = totalLighting;

    if(u_DiffuseTextureEnabled) {
        vec4 texColor = texture(u_TextureDiffuse, v_TexCoord);
        finalColor *= texColor;
    }

    finalColor.a *= u_Material.Alpha;

    return finalColor;
}

void main() {
    FragColor = CalcPhongLighting();

    if(FragColor.a < 0.01) {
        discard;
    }

    if(u_FogEnabled) {
        float fogFactor = CalcFogFactor();
        FragColor = mix(vec4(u_Fog.Color, 1.0), FragColor, fogFactor);
    }
}