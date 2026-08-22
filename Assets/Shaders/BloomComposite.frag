#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_HDRTexture;
uniform sampler2D u_BloomTexture;

uniform float u_BloomStrength;

void main()
{
    vec3 hdrColor =
        texture(
            u_HDRTexture,
            v_TexCoord).rgb;

    vec3 bloomColor =
        texture(
            u_BloomTexture,
            v_TexCoord).rgb;


    vec3 result =
        hdrColor +
        bloomColor *
        u_BloomStrength;


    FragColor =
        vec4(
            result,
            1.0);
}