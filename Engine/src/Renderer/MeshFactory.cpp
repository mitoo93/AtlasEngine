#include "AtlasEngine/Renderer/MeshFactory.h"
#include "AtlasEngine/Renderer/Vertex.h"
#include <vector>

namespace AtlasEngine
{

void MeshFactory::CreateQuad(Mesh& mesh)
{
    std::vector<Vertex> vertices
    {
        { { -0.5f, -0.5f, 0.0f } },
        { {  0.5f, -0.5f, 0.0f } },
        { {  0.5f,  0.5f, 0.0f } },
        { { -0.5f,  0.5f, 0.0f } }
    };

    std::vector<unsigned int> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    mesh.Create(
        vertices,
        indices,
        Vertex::GetLayout());
}

}