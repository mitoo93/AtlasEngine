#version 330 core

in vec2 v_TexCoord;

out vec2 FragColor;

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


    return
        normalize(
            tangent *
            halfVector.x +
            bitangent *
            halfVector.y +
            normal *
            halfVector.z);
}


float GeometrySchlickGGXIBL(
    float nDotV,
    float roughness)
{
    float a =
        roughness;

    float k =
        (
            a *
            a
        ) /
        2.0;

    return
        nDotV /
        max(
            nDotV *
            (1.0 - k) +
            k,
            0.000001);
}


float GeometrySmithIBL(
    vec3 normal,
    vec3 view,
    vec3 light,
    float roughness)
{
    float nDotV =
        max(
            dot(
                normal,
                view),
            0.0);

    float nDotL =
        max(
            dot(
                normal,
                light),
            0.0);


    float ggxView =
        GeometrySchlickGGXIBL(
            nDotV,
            roughness);

    float ggxLight =
        GeometrySchlickGGXIBL(
            nDotL,
            roughness);


    return
        ggxView *
        ggxLight;
}


vec2 IntegrateBRDF(
    float nDotV,
    float roughness)
{
    vec3 view;

    view.x =
        sqrt(
            max(
                1.0 -
                nDotV *
                nDotV,
                0.0));

    view.y =
        0.0;

    view.z =
        nDotV;


    float A =
        0.0;

    float B =
        0.0;


    vec3 normal =
        vec3(
            0.0,
            0.0,
            1.0);


    const uint sampleCount =
        1024u;


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
                roughness);


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
                light.z,
                0.0);

        float nDotH =
            max(
                halfVector.z,
                0.0);

        float vDotH =
            max(
                dot(
                    view,
                    halfVector),
                0.0);


        if (nDotL > 0.0)
        {
            float geometry =
                GeometrySmithIBL(
                    normal,
                    view,
                    light,
                    roughness);


            float geometryVisible =
                (
                    geometry *
                    vDotH
                ) /
                max(
                    nDotH *
                    nDotV,
                    0.000001);


            float fresnel =
                pow(
                    1.0 -
                    vDotH,
                    5.0);


            A +=
                (1.0 - fresnel) *
                geometryVisible;

            B +=
                fresnel *
                geometryVisible;
        }
    }


    A /=
        float(sampleCount);

    B /=
        float(sampleCount);


    return
        vec2(
            A,
            B);
}


void main()
{
    FragColor =
        IntegrateBRDF(
            v_TexCoord.x,
            v_TexCoord.y);
}