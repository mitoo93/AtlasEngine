#version 330 core

in vec3 v_LocalPosition;

out vec4 FragColor;

uniform samplerCube u_EnvironmentMap;
uniform float u_Roughness;

#define PI 3.14159265359

float RadicalInverseVanDerCorput(
    uint bits)
{
    bits =
        (bits << 16u) |
        (bits >> 16u);

    bits =
        ((bits & 0x55555555u) << 1u) |
        ((bits & 0xAAAAAAAAu) >> 1u);

    bits =
        ((bits & 0x33333333u) << 2u) |
        ((bits & 0xCCCCCCCCu) >> 2u);

    bits =
        ((bits & 0x0F0F0F0Fu) << 4u) |
        ((bits & 0xF0F0F0F0u) >> 4u);

    bits =
        ((bits & 0x00FF00FFu) << 8u) |
        ((bits & 0xFF00FF00u) >> 8u);

    return
        float(bits) *
        2.3283064365386963e-10;
}


vec2 Hammersley(
    uint i,
    uint sampleCount)
{
    return
        vec2(
            float(i) /
            float(sampleCount),

            RadicalInverseVanDerCorput(
                i));
}


vec3 ImportanceSampleGGX(
    vec2 xi,
    vec3 normal,
    float roughness)
{
    float a =
        roughness *
        roughness;

    float phi =
        2.0 *
        PI *
        xi.x;

    float cosTheta =
        sqrt(
            (1.0 - xi.y) /
            (
                1.0 +
                (
                    a *
                    a -
                    1.0
                ) *
                xi.y
            ));

    float sinTheta =
        sqrt(
            max(
                1.0 -
                cosTheta *
                cosTheta,
                0.0));


    vec3 halfVector =
        vec3(
            cos(phi) *
            sinTheta,

            sin(phi) *
            sinTheta,

            cosTheta);


    vec3 up =
        abs(normal.z) <
        0.999
            ? vec3(
                0.0,
                0.0,
                1.0)
            : vec3(
                1.0,
                0.0,
                0.0);


    vec3 tangent =
        normalize(
            cross(
                up,
                normal));

    vec3 bitangent =
        cross(
            normal,
            tangent);


    vec3 sampleVector =
        tangent *
        halfVector.x +
        bitangent *
        halfVector.y +
        normal *
        halfVector.z;


    return
        normalize(
            sampleVector);
}


void main()
{
    vec3 normal =
        normalize(
            v_LocalPosition);

    vec3 reflection =
        normal;

    vec3 view =
        reflection;


    const uint sampleCount =
        1024u;


    vec3 prefilteredColor =
        vec3(0.0);

    float totalWeight =
        0.0;


    for (uint i = 0u;
         i < sampleCount;
         ++i)
    {
        vec2 xi =
            Hammersley(
                i,
                sampleCount);


        vec3 halfVector =
            ImportanceSampleGGX(
                xi,
                normal,
                u_Roughness);


        vec3 light =
            normalize(
                2.0 *
                dot(
                    view,
                    halfVector) *
                halfVector -
                view);


        float nDotL =
            max(
                dot(
                    normal,
                    light),
                0.0);


        if (nDotL > 0.0)
        {
            prefilteredColor +=
                texture(
                    u_EnvironmentMap,
                    light).rgb *
                nDotL;

            totalWeight +=
                nDotL;
        }
    }


    prefilteredColor =
        prefilteredColor /
        max(
            totalWeight,
            0.0001);


    FragColor =
        vec4(
            prefilteredColor,
            1.0);
}