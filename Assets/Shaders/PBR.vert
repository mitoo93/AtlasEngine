#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPosition;
out mat3 v_TBN;

void main()
{
    vec4 worldPosition =
        u_Model *
        vec4(
            a_Position,
            1.0);

    v_FragPosition =
        worldPosition.xyz;

    v_TexCoord =
        a_TexCoord;

    mat3 normalMatrix =
        transpose(
            inverse(
                mat3(
                    u_Model)));

    vec3 normal =
        normalize(
            normalMatrix *
            a_Normal);

    vec3 tangent =
        normalize(
            normalMatrix *
            a_Tangent);

    tangent =
        normalize(
            tangent -
            dot(
                tangent,
                normal) *
            normal);

    vec3 bitangent =
        normalize(
            cross(
                normal,
                tangent));

    v_Normal =
        normal;

    v_TBN =
        mat3(
            tangent,
            bitangent,
            normal);

    gl_Position =
        u_Projection *
        u_View *
        worldPosition;
}