#version 330 core

in vec3 v_TexCoord;

out vec4 FragColor;

uniform samplerCube u_Skybox;

void main()
{
    vec3 color =
        texture(
            u_Skybox,
            v_TexCoord).rgb;

    FragColor =
        vec4(
            color,
            1.0);
}