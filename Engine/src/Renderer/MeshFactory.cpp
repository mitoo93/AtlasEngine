#include "AtlasEngine/Renderer/MeshFactory.h"

#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Vertex.h"

#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

namespace AtlasEngine
{

std::unique_ptr<Mesh> MeshFactory::CreateQuad()
{
    auto mesh =
        std::make_unique<Mesh>();

    std::vector<Vertex> vertices
    {
        {
            { -0.5f, -0.5f, 0.0f },
            {  0.0f,  0.0f, 1.0f },
            { 0.0f, 0.0f }
        },

        {
            {  0.5f, -0.5f, 0.0f },
            {  0.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f }
        },

        {
            {  0.5f,  0.5f, 0.0f },
            {  0.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f }
        },

        {
            { -0.5f,  0.5f, 0.0f },
            {  0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f }
        }
    };

    std::vector<unsigned int> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    mesh->Create(
        vertices,
        indices,
        Vertex::GetLayout());

    return mesh;
}

std::unique_ptr<Mesh> MeshFactory::CreateSphere(
    float radius,
    unsigned int segments,
    unsigned int rings)
{
    auto mesh =
        std::make_unique<Mesh>();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float pi =
        3.14159265359f;

    for (unsigned int y = 0;
         y <= rings;
         ++y)
    {
        float v =
            static_cast<float>(y) /
            static_cast<float>(rings);

        float phi =
            v * pi;

        for (unsigned int x = 0;
             x <= segments;
             ++x)
        {
            float u =
                static_cast<float>(x) /
                static_cast<float>(segments);

            float theta =
                u *
                2.0f *
                pi;

            float sinPhi =
                std::sin(phi);

            float cosPhi =
                std::cos(phi);

            float sinTheta =
                std::sin(theta);

            float cosTheta =
                std::cos(theta);

            Vertex vertex{};

            //------------------------------------------------
            // Position
            //------------------------------------------------

            vertex.Position[0] =
                radius *
                sinPhi *
                cosTheta;

            vertex.Position[1] =
                radius *
                cosPhi;

            vertex.Position[2] =
                radius *
                sinPhi *
                sinTheta;

            //------------------------------------------------
            // Normal
            //------------------------------------------------

            vertex.Normal[0] =
                sinPhi *
                cosTheta;

            vertex.Normal[1] =
                cosPhi;

            vertex.Normal[2] =
                sinPhi *
                sinTheta;

            //------------------------------------------------
            // UV
            //------------------------------------------------

            vertex.TexCoord[0] =
                u;

            vertex.TexCoord[1] =
                v;

            //------------------------------------------------
            // Tangent
            //------------------------------------------------

            vertex.Tangent[0] =
                -sinTheta;

            vertex.Tangent[1] =
                0.0f;

            vertex.Tangent[2] =
                cosTheta;

            //------------------------------------------------
            // Bitangent
            //------------------------------------------------

            vertex.Bitangent[0] =
                cosPhi *
                cosTheta;

            vertex.Bitangent[1] =
                -sinPhi;

            vertex.Bitangent[2] =
                cosPhi *
                sinTheta;

            vertices.push_back(
                vertex);
        }
    }

    //----------------------------------------------------
    // Indices
    //----------------------------------------------------

    for (unsigned int y = 0;
         y < rings;
         ++y)
    {
        for (unsigned int x = 0;
             x < segments;
             ++x)
        {
            unsigned int current =
                y *
                (segments + 1) +
                x;

            unsigned int next =
                current +
                segments +
                1;

            indices.push_back(
                current);

            indices.push_back(
                current + 1);

            indices.push_back(
                next);

            indices.push_back(
                current + 1);

            indices.push_back(
                next + 1);

            indices.push_back(
                next);
        }
    }

    mesh->Create(
        vertices,
        indices,
        Vertex::GetLayout());

    mesh->SetName(
        "Sphere");

    return mesh;
}

}