#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_HDRTexture;

uniform float u_Exposure;

uniform bool u_EnableGrayscale;
uniform bool u_EnableVignette;

uniform float u_VignetteStrength;

void main()
{
    // ------------------------------------------------
    // HDR INPUT
    // ------------------------------------------------

    vec3 hdrColor =
        texture(
            u_HDRTexture,
            v_TexCoord).rgb;


    // ------------------------------------------------
    // TONE MAPPING
    // ------------------------------------------------

    vec3 color =
        vec3(1.0) -
        exp(
            -hdrColor *
            u_Exposure);


    // ------------------------------------------------
    // GRAYSCALE
    // ------------------------------------------------

    if (u_EnableGrayscale)
    {
        float luminance =
            dot(
                color,
                vec3(
                    0.2126,
                    0.7152,
                    0.0722));

        color =
            vec3(
                luminance);
    }


    // ------------------------------------------------
    // VIGNETTE
    // ------------------------------------------------

    if (u_EnableVignette)
    {
        vec2 centeredUV =
            v_TexCoord -
            vec2(0.5);

        float distanceFromCenter =
            length(
                centeredUV);

        float vignette =
            1.0 -
            distanceFromCenter *
            u_VignetteStrength;

        vignette =
            clamp(
                vignette,
                0.0,
                1.0);

        color *=
            vignette;
    }


    // ------------------------------------------------
    // GAMMA CORRECTION
    // ------------------------------------------------

    const float gamma =
        2.2;

    color =
        pow(
            color,
            vec3(
                1.0 /
                gamma));


    FragColor =
        vec4(
            color,
            1.0);
}