#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPosition;
in mat3 v_TBN;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform bool u_HasTexture;

uniform sampler2D u_NormalTexture;
uniform bool u_HasNormalTexture;

uniform sampler2D u_MetallicTexture;
uniform bool u_HasMetallicTexture;

uniform sampler2D u_RoughnessTexture;
uniform bool u_HasRoughnessTexture;

uniform sampler2D u_AOTexture;
uniform bool u_HasAOTexture;

uniform vec3 u_BaseColor;

uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_AO;

uniform vec3 u_ViewPosition;

uniform bool u_PackedMetallicRoughness;

uniform samplerCube u_IrradianceMap;
uniform bool u_HasIrradianceMap;

uniform samplerCube u_PrefilterMap;
uniform bool u_HasPrefilterMap;

uniform sampler2D u_BRDFLUT;
uniform bool u_HasBRDFLUT;


// ----------------------------------------------------
// DIRECTIONAL LIGHT
// ----------------------------------------------------

uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;


// ----------------------------------------------------
// AMBIENT LIGHT
// ----------------------------------------------------

uniform vec3 u_AmbientColor;
uniform float u_AmbientIntensity;


// ----------------------------------------------------
// POINT LIGHTS
// ----------------------------------------------------

#define MAX_POINT_LIGHTS 4
#define PI 3.14159265359

struct PointLight
{
    vec3 Position;
    vec3 Color;

    float Intensity;

    float Constant;
    float Linear;
    float Quadratic;
};

uniform int u_PointLightCount;

uniform PointLight
    u_PointLights[MAX_POINT_LIGHTS];


// ----------------------------------------------------
// GGX DISTRIBUTION
// ----------------------------------------------------

float DistributionGGX(
    vec3 normal,
    vec3 halfVector,
    float roughness)
{
    float a =
        roughness *
        roughness;

    float a2 =
        a *
        a;

    float nDotH =
        max(
            dot(
                normal,
                halfVector),
            0.0);

    float nDotH2 =
        nDotH *
        nDotH;

    float numerator =
        a2;

    float denominator =
        (
            nDotH2 *
            (a2 - 1.0) +
            1.0
        );

    denominator =
        PI *
        denominator *
        denominator;

    return numerator /
        max(
            denominator,
            0.000001);
}


// ----------------------------------------------------
// GEOMETRY
// ----------------------------------------------------

float GeometrySchlickGGX(
    float nDotV,
    float roughness)
{
    float r =
        roughness +
        1.0;

    float k =
        (
            r *
            r
        ) /
        8.0;

    float numerator =
        nDotV;

    float denominator =
        nDotV *
        (1.0 - k) +
        k;

    return numerator /
        max(
            denominator,
            0.000001);
}


float GeometrySmith(
    vec3 normal,
    vec3 viewDirection,
    vec3 lightDirection,
    float roughness)
{
    float nDotV =
        max(
            dot(
                normal,
                viewDirection),
            0.0);

    float nDotL =
        max(
            dot(
                normal,
                lightDirection),
            0.0);

    float ggxView =
        GeometrySchlickGGX(
            nDotV,
            roughness);

    float ggxLight =
        GeometrySchlickGGX(
            nDotL,
            roughness);

    return
        ggxView *
        ggxLight;
}


// ----------------------------------------------------
// FRESNEL
// ----------------------------------------------------

vec3 FresnelSchlick(
    float cosTheta,
    vec3 f0)
{
    return
        f0 +
        (
            vec3(1.0) -
            f0
        ) *
        pow(
            1.0 -
            cosTheta,
            5.0);
}

vec3 FresnelSchlickRoughness(
    float cosTheta,
    vec3 f0,
    float roughness)
{
    return
        f0 +
        (
            max(
                vec3(
                    1.0 - roughness),
                f0) -
            f0
        ) *
        pow(
            1.0 - cosTheta,
            5.0);
}


// ----------------------------------------------------
// MAIN
// ----------------------------------------------------

void main()
{
    vec3 albedo =
        u_BaseColor;

    if (u_HasTexture)
    {
        albedo =
            texture(
                u_Texture,
                v_TexCoord).rgb;
    }


    vec3 normal;

    if (u_HasNormalTexture)
    {
        vec3 tangentNormal =
            texture(
                u_NormalTexture,
                v_TexCoord).rgb;

        tangentNormal =
            tangentNormal *
            2.0 -
            1.0;

        normal =
            normalize(
                v_TBN *
                tangentNormal);
    }
    else
    {
        normal =
            normalize(
                v_Normal);
    }


    vec3 viewDirection =
        normalize(
            u_ViewPosition -
            v_FragPosition);


    // ------------------------------------------------
    // MATERIAL
    // ------------------------------------------------

    float metallic =
        u_Metallic;

    if (u_HasMetallicTexture)
    {
        vec4 metallicSample =
            texture(
                u_MetallicTexture,
                v_TexCoord);

        metallic =
            u_PackedMetallicRoughness
                ? metallicSample.b
                : metallicSample.r;
    }

    metallic =
        clamp(
            metallic,
            0.0,
            1.0);


    float roughness =
        u_Roughness;

    if (u_HasRoughnessTexture)
    {
        vec4 roughnessSample =
            texture(
                u_RoughnessTexture,
                v_TexCoord);

        roughness =
            u_PackedMetallicRoughness
                ? roughnessSample.g
                : roughnessSample.r;
    }

    roughness =
        clamp(
            roughness,
            0.04,
            1.0);

    float ao =
        u_AO;

    if (u_HasAOTexture)
    {
        ao =
            texture(
                u_AOTexture,
                v_TexCoord).r;
    }

    ao =
        clamp(
            ao,
            0.0,
            1.0);


    // ------------------------------------------------
    // F0
    // ------------------------------------------------

    vec3 f0 =
        vec3(
            0.04);

    f0 =
        mix(
            f0,
            albedo,
            metallic);

    float nDotV =
        max(
            dot(
                normal,
                viewDirection),
            0.0);


    vec3 iblFresnel =
        FresnelSchlickRoughness(
            nDotV,
            f0,
            roughness);


    vec3 iblKS =
        iblFresnel;


    vec3 iblKD =
        vec3(1.0) -
        iblKS;

    iblKD *=
        1.0 -
        metallic;


    // ------------------------------------------------
    // DIFFUSE IBL
    // ------------------------------------------------

    vec3 diffuseIBL =
        vec3(0.0);

    if (u_HasIrradianceMap)
    {
        vec3 irradiance =
            texture(
                u_IrradianceMap,
                normal).rgb;

        diffuseIBL =
            irradiance *
            albedo;
    }


    // ------------------------------------------------
    // SPECULAR IBL
    // ------------------------------------------------

    vec3 specularIBL =
        vec3(0.0);

    if (u_HasPrefilterMap &&
        u_HasBRDFLUT)
    {
        vec3 reflectionDirection =
            reflect(
                -viewDirection,
                normal);


        const float maxReflectionLOD =
            4.0;


        vec3 prefilteredColor =
            textureLod(
                u_PrefilterMap,
                reflectionDirection,
                roughness *
                maxReflectionLOD).rgb;


        vec2 brdf =
            texture(
                u_BRDFLUT,
                vec2(
                    nDotV,
                    roughness)).rg;


        specularIBL =
            prefilteredColor *
            (
                iblFresnel *
                brdf.x +
                brdf.y
            );
    }


    // ------------------------------------------------
    // DIRECTIONAL LIGHT
    // ------------------------------------------------

    vec3 directionalLightDirection =
        normalize(
            -u_LightDirection);

    vec3 directionalHalfVector =
        normalize(
            viewDirection +
            directionalLightDirection);

    float directionalNDF =
        DistributionGGX(
            normal,
            directionalHalfVector,
            roughness);

    float directionalGeometry =
        GeometrySmith(
            normal,
            viewDirection,
            directionalLightDirection,
            roughness);

    vec3 directionalFresnel =
        FresnelSchlick(
            max(
                dot(
                    directionalHalfVector,
                    viewDirection),
                0.0),
            f0);

    vec3 directionalNumerator =
        directionalNDF *
        directionalGeometry *
        directionalFresnel;

    float directionalDenominator =
        4.0 *
        max(
            dot(
                normal,
                viewDirection),
            0.0) *
        max(
            dot(
                normal,
                directionalLightDirection),
            0.0);

    vec3 directionalSpecular =
        directionalNumerator /
        max(
            directionalDenominator,
            0.0001);

    vec3 directionalKS =
        directionalFresnel;

    vec3 directionalKD =
        vec3(1.0) -
        directionalKS;

    directionalKD *=
        1.0 -
        metallic;

    float directionalNDotL =
        max(
            dot(
                normal,
                directionalLightDirection),
            0.0);

    vec3 directionalRadiance =
        u_LightColor *
        u_LightIntensity;

    vec3 directionalContribution =
        (
            directionalKD *
            albedo /
            PI +
            directionalSpecular
        ) *
        directionalRadiance *
        directionalNDotL;


    // ------------------------------------------------
    // POINT LIGHTS
    // ------------------------------------------------

    vec3 lightContribution =
        vec3(
            0.0);

    for (int i = 0;
         i < u_PointLightCount &&
         i < MAX_POINT_LIGHTS;
         ++i)
    {
        PointLight pointLight =
            u_PointLights[i];

        vec3 lightVector =
            pointLight.Position -
            v_FragPosition;

        float distance =
            length(
                lightVector);

        vec3 lightDirection =
            normalize(
                lightVector);

        vec3 halfVector =
            normalize(
                viewDirection +
                lightDirection);

        float attenuation =
            1.0 /
            (
                pointLight.Constant +
                pointLight.Linear *
                distance +
                pointLight.Quadratic *
                distance *
                distance
            );

        vec3 radiance =
            pointLight.Color *
            pointLight.Intensity *
            attenuation;

        float ndf =
            DistributionGGX(
                normal,
                halfVector,
                roughness);

        float geometry =
            GeometrySmith(
                normal,
                viewDirection,
                lightDirection,
                roughness);

        vec3 fresnel =
            FresnelSchlick(
                max(
                    dot(
                        halfVector,
                        viewDirection),
                    0.0),
                f0);

        vec3 numerator =
            ndf *
            geometry *
            fresnel;

        float denominator =
            4.0 *
            max(
                dot(
                    normal,
                    viewDirection),
                0.0) *
            max(
                dot(
                    normal,
                    lightDirection),
                0.0);

        vec3 specular =
            numerator /
            max(
                denominator,
                0.0001);

        vec3 kS =
            fresnel;

        vec3 kD =
            vec3(1.0) -
            kS;

        kD *=
            1.0 -
            metallic;

        float nDotL =
            max(
                dot(
                    normal,
                    lightDirection),
                0.0);

        lightContribution +=
            (
                kD *
                albedo /
                PI +
                specular
            ) *
            radiance *
            nDotL;
    }


    // ------------------------------------------------
    // AMBIENT
    // ------------------------------------------------

    vec3 ambient;

    if (u_HasIrradianceMap &&
        u_HasPrefilterMap &&
        u_HasBRDFLUT)
    {
        ambient =
            (
                iblKD *
                diffuseIBL +
                specularIBL
            ) *
            ao;
    }
    else
    {
        ambient =
            u_AmbientColor *
            u_AmbientIntensity *
            albedo *
            ao;
    }


    // ------------------------------------------------
    // FINAL
    // ------------------------------------------------

    vec3 color =
        ambient +
        directionalContribution +
        lightContribution;

    FragColor =
        vec4(
            color,
            1.0);
}