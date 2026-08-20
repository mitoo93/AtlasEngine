#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPosition;
out mat3 v_TBN;
out vec4 v_FragPositionLightSpace;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix;

void main()
{
    // Posicion del vertice en espacio de mundo
    vec4 worldPosition =
        u_Model *
        vec4(a_Position, 1.0);

    v_FragPositionLightSpace =
        u_LightSpaceMatrix *
        worldPosition;

    v_FragPosition =
        worldPosition.xyz;

    // Las normales no se deben transformar simplemente
    // con u_Model cuando existe escala no uniforme.
    mat3 normalMatrix =
        transpose(
            inverse(
                mat3(u_Model)));
    
    vec3 T =
        normalize(
            normalMatrix *
            a_Tangent);

    vec3 B =
        normalize(
            normalMatrix *
            a_Bitangent);

    vec3 N =
        normalize(
            normalMatrix *
            a_Normal);

    v_Normal =
        N;

    v_TBN =
        mat3(
            T,
            B,
            N);

    v_TexCoord =
        a_TexCoord;

    gl_Position =
        u_Projection *
        u_View *
        worldPosition;
}