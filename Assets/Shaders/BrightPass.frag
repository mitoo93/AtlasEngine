#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_HDRTexture;

uniform float u_Threshold;

void main()
{
    vec3 hdrColor =
        texture(
            u_HDRTexture,
            v_TexCoord).rgb;


    // ------------------------------------------------
    // LUMINANCE
    // ------------------------------------------------

    float brightness =
        dot(
            hdrColor,
            vec3(
                0.2126,
                0.7152,
                0.0722));


    // ------------------------------------------------
    // BRIGHT PASS
    // ------------------------------------------------

    if (brightness >
        u_Threshold)
    {
        FragColor =
            vec4(
                hdrColor,
                1.0);
    }
    else
    {
        FragColor =
            vec4(
                0.0,
                0.0,
                0.0,
                1.0);
    }
}