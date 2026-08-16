#include "AtlasEngine/Renderer/MeshFactory.h"

#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Vertex.h"

#include <vector>
#include <memory>

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

}