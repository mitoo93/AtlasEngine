#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform bool u_HasTexture;
uniform vec3 u_BaseColor;

void main()
{
    if (u_HasTexture)
    {
        FragColor =
            texture(
                u_Texture,
                v_TexCoord);
    }
    else
    {
        FragColor =
            vec4(
                u_BaseColor,
                1.0);
    }
}