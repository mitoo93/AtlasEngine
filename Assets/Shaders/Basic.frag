#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPosition;
in mat3 v_TBN;
in vec4 v_FragPositionLightSpace;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform bool u_HasTexture;
uniform vec3 u_BaseColor;

uniform sampler2D u_NormalTexture;
uniform bool u_HasNormalTexture;

uniform sampler2D u_SpecularTexture;
uniform bool u_HasSpecularTexture;

uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;

uniform vec3 u_AmbientColor;
uniform float u_AmbientIntensity;

uniform vec3 u_ViewPosition;
uniform float u_Shininess;
uniform float u_SpecularStrength;
uniform sampler2D u_ShadowMap;

#define MAX_POINT_LIGHTS 4

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

uniform vec3 u_SpotLightPosition;
uniform vec3 u_SpotLightDirection;
uniform vec3 u_SpotLightColor;

uniform float u_SpotLightIntensity;

uniform float u_SpotLightInnerCutoff;
uniform float u_SpotLightOuterCutoff;

float CalculateShadow(
    vec4 fragPositionLightSpace,
    vec3 normal,
    vec3 lightDirection)
{
    vec3 projectionCoordinates =
        fragPositionLightSpace.xyz /
        fragPositionLightSpace.w;

    projectionCoordinates =
        projectionCoordinates *
        0.5 +
        0.5;

    float currentDepth =
        projectionCoordinates.z;

    float bias =
        max(
            0.005 *
            (
                1.0 -
                dot(
                    normal,
                    -lightDirection)
            ),
            0.0005);

    vec2 texelSize =
        1.0 /
        textureSize(
            u_ShadowMap,
            0);

    float shadow =
        0.0;

    for (int x = -2;
        x <= 2;
        ++x)
    {
        for (int y = -2;
            y <= 2;
            ++y)
        {
            float pcfDepth =
                texture(
                    u_ShadowMap,
                    projectionCoordinates.xy +
                    vec2(x, y) *
                    texelSize).r;

            shadow +=
                currentDepth -
                bias >
                pcfDepth
                    ? 1.0
                    : 0.0;
        }
    }

    shadow /=
        25.0;

    if (projectionCoordinates.z > 1.0 ||
        projectionCoordinates.x < 0.0 ||
        projectionCoordinates.x > 1.0 ||
        projectionCoordinates.y < 0.0 ||
        projectionCoordinates.y > 1.0)
    {
        shadow =
            0.0;
    }

    return shadow;
}

void main()
{
    vec3 baseColor;

    if (u_HasTexture)
    {
        baseColor =
            texture(
                u_Texture,
                v_TexCoord).rgb;
    }
    else
    {
        baseColor =
            u_BaseColor;
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

    // ----------------------------------------------------
    // SPECULAR MAP
    // ----------------------------------------------------

    float materialSpecularStrength =
        u_SpecularStrength;

    if (u_HasSpecularTexture)
    {
        materialSpecularStrength *=
            texture(
                u_SpecularTexture,
                v_TexCoord).r;
    }

    vec3 lightDirection =
        normalize(
            u_LightDirection);

    float shadow =
        CalculateShadow(
            v_FragPositionLightSpace,
            normal,
            lightDirection);

    float diffuseStrength =
        max(
            dot(
                normal,
                -lightDirection),
            0.0);

    vec3 ambient =
        u_AmbientColor *
        u_AmbientIntensity *
        baseColor;

    vec3 diffuse =
        diffuseStrength *
        u_LightColor *
        u_LightIntensity *
        baseColor;

    vec3 viewDirection =
        normalize(
            u_ViewPosition -
            v_FragPosition);

    vec3 halfDirection =
        normalize(
            -lightDirection +
            viewDirection);

    float specularStrength =
        pow(
            max(
                dot(
                    normal,
                    halfDirection),
                0.0),
            u_Shininess);

    vec3 specular =
        specularStrength *
        u_LightColor *
        u_LightIntensity *
        materialSpecularStrength;

    vec3 pointContribution =
        vec3(0.0);

    for (int i = 0;
        i < u_PointLightCount &&
        i < MAX_POINT_LIGHTS;
        ++i)
    {
        PointLight pointLight =
            u_PointLights[i];

        vec3 pointDirection =
            pointLight.Position -
            v_FragPosition;

        float pointDistance =
            length(
                pointDirection);

        pointDirection =
            normalize(
                pointDirection);

        float pointDiffuseStrength =
            max(
                dot(
                    normal,
                    pointDirection),
                0.0);

        float pointSpecularStrength =
            0.0;

        if (pointDiffuseStrength > 0.0)
        {
            vec3 pointHalfDirection =
                normalize(
                    pointDirection +
                    viewDirection);

            pointSpecularStrength =
                pow(
                    max(
                        dot(
                            normal,
                            pointHalfDirection),
                        0.0),
                    u_Shininess);
        }

        float attenuation =
            1.0 /
            (
                pointLight.Constant +
                pointLight.Linear *
                pointDistance +
                pointLight.Quadratic *
                pointDistance *
                pointDistance
            );

        vec3 pointDiffuse =
            pointDiffuseStrength *
            pointLight.Color *
            pointLight.Intensity *
            baseColor;

        vec3 pointSpecular =
            pointSpecularStrength *
            pointLight.Color *
            pointLight.Intensity *
            materialSpecularStrength;

        pointContribution +=
            (
                pointDiffuse +
                pointSpecular
            )
            *
            attenuation;
    }

    // ----------------------------------------------------
    // SPOT LIGHT
    // ----------------------------------------------------

    vec3 spotDirection =
        u_SpotLightPosition -
        v_FragPosition;

    float spotDistance =
        length(
            spotDirection);

    spotDirection =
        normalize(
            spotDirection);

    float theta =
        dot(
            spotDirection,
            normalize(
                -u_SpotLightDirection));

    float innerCutoff =
        cos(
            radians(
                u_SpotLightInnerCutoff));

    float outerCutoff =
        cos(
            radians(
                u_SpotLightOuterCutoff));

    float epsilon =
        innerCutoff -
        outerCutoff;

    float spotFactor =
        clamp(
            (theta - outerCutoff) /
            epsilon,
            0.0,
            1.0);

    float spotDiffuseStrength =
        max(
            dot(
                normal,
                spotDirection),
            0.0);

    vec3 spotHalfDirection =
        normalize(
            spotDirection +
            viewDirection);

    float spotSpecularStrength =
        pow(
            max(
                dot(
                    normal,
                    spotHalfDirection),
                0.0),
            u_Shininess);

    vec3 spotDiffuse =
        spotDiffuseStrength *
        u_SpotLightColor *
        u_SpotLightIntensity *
        baseColor;

    vec3 spotSpecular =
        spotSpecularStrength *
        u_SpotLightColor *
        u_SpotLightIntensity *
        materialSpecularStrength;

    vec3 spotContribution =
        (
            spotDiffuse +
            spotSpecular
        )
        *
        spotFactor;

    vec3 finalColor =
        ambient +
        (
            1.0 -
            shadow
        )
        *
        (
            diffuse +
            specular
        )
        +
        pointContribution +
        spotContribution;

    FragColor =
        vec4(
            finalColor,
            1.0);
}