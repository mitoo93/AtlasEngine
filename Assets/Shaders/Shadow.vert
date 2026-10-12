#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 5) in mat4 a_InstanceModel;

uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

uniform bool u_UseInstancing;

void main()
{
    mat4 modelMatrix = u_Model;

    if (u_UseInstancing)
    {
        modelMatrix = a_InstanceModel;
    }

    gl_Position =
        u_LightSpaceMatrix *
        modelMatrix *
        vec4(a_Position, 1.0);
}