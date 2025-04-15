#version 330

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;
layout(location=2) in vec3 a_Normal;
layout(location=3) in vec3 a_Tangent;
layout(location=4) in vec3 a_Bitangent;

uniform mat4 u_PVM;
uniform mat4 u_Model;
uniform mat4 u_LightPVM;
uniform vec4 u_ClipPlane;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPosition;
out vec4 v_LightSpacePosition;
out vec3 v_Tangent;
out vec3 v_Bitangent;

void main() {
    vec4 worldPosition = u_Model * vec4(a_Position, 1.0);

    v_TexCoord = a_TexCoord;
    v_Normal = mat3(u_Model) * a_Normal;
    v_Tangent = mat3(u_Model) * a_Tangent;
    v_Bitangent = mat3(u_Model) * a_Bitangent;
    v_WorldPosition = worldPosition.xyz;
    v_LightSpacePosition = u_LightPVM * vec4(a_Position, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, u_ClipPlane);
    gl_Position = u_PVM * vec4(a_Position, 1.0);
}