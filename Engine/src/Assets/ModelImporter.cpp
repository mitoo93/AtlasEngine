#include "AtlasEngine/Assets/ModelImporter.h"

#include "AtlasEngine/Renderer/Model.h"
#include "AtlasEngine/Renderer/Mesh.h"
#include "AtlasEngine/Renderer/Vertex.h"
#include "AtlasEngine/Renderer/Material.h"
#include "AtlasEngine/Renderer/Shader.h"
#include "AtlasEngine/Renderer/Texture.h"
#include "AtlasEngine/Assets/AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <filesystem>

namespace AtlasEngine
{

namespace
{

std::unique_ptr<Mesh> ProcessMesh(
    aiMesh* mesh,
    const aiScene* scene,
    const std::filesystem::path& modelDirectory)
{
    if (!mesh)
        return nullptr;

    if (mesh->mNumBones > 0)
    {
        std::cout
            << "AVISO: Mesh skinned detectado: "
            << mesh->mName.C_Str()
            << " | Bones: "
            << mesh->mNumBones
            << " | Skinning pendiente del sistema de animacion"
            << '\n';
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0;
         i < mesh->mNumVertices;
         ++i)
    {
        Vertex vertex{};

        vertex.Position[0] =
            mesh->mVertices[i].x;

        vertex.Position[1] =
            mesh->mVertices[i].y;

        vertex.Position[2] =
            mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.Normal[0] =
                mesh->mNormals[i].x;

            vertex.Normal[1] =
                mesh->mNormals[i].y;

            vertex.Normal[2] =
                mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoord[0] =
                mesh->mTextureCoords[0][i].x;

            vertex.TexCoord[1] =
                mesh->mTextureCoords[0][i].y;

        }
        else
        {
            vertex.TexCoord[0] = 0.0f;
            vertex.TexCoord[1] = 0.0f;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0;
         i < mesh->mNumFaces;
         ++i)
    {
        const aiFace& face =
            mesh->mFaces[i];

        for (unsigned int j = 0;
             j < face.mNumIndices;
             ++j)
        {
            indices.push_back(
                face.mIndices[j]);
        }
    }

    float minX = vertices[0].Position[0];
    float minY = vertices[0].Position[1];
    float minZ = vertices[0].Position[2];

    float maxX = vertices[0].Position[0];
    float maxY = vertices[0].Position[1];
    float maxZ = vertices[0].Position[2];

    for (const Vertex& vertex : vertices)
    {
        minX = std::min(minX, vertex.Position[0]);
        minY = std::min(minY, vertex.Position[1]);
        minZ = std::min(minZ, vertex.Position[2]);

        maxX = std::max(maxX, vertex.Position[0]);
        maxY = std::max(maxY, vertex.Position[1]);
        maxZ = std::max(maxZ, vertex.Position[2]);
    }
    
    auto result =
        std::make_unique<Mesh>();

    if (!result->Create(
            vertices,
            indices,
            Vertex::GetLayout()))
    {
        return nullptr;
    }

    result->SetBounds(
        Vector3{
            minX,
            minY,
            minZ
        },
        Vector3{
            maxX,
            maxY,
            maxZ
        });

    result->SetName(
        mesh->mName.C_Str());

    if (scene &&
        mesh->mMaterialIndex <
            scene->mNumMaterials)
    {
        aiMaterial* aiMat =
            scene->mMaterials[
                mesh->mMaterialIndex];

        if (aiMat)
        {
            aiString materialName;

            aiMat->Get(
                AI_MATKEY_NAME,
                materialName);

            std::string materialAssetName =
                std::string("Model_") +
                materialName.C_Str();

            std::cout
                << "Material importado: "
                << materialName.C_Str()
                << std::endl;

            Shader* shader =
                AssetManager::GetShader("Basic");

            if (shader)
            {
                // ------------------------------------------------
                // MATERIAL
                // ------------------------------------------------

                if (!AssetManager::GetMaterial(
                        materialAssetName))
                {
                    AssetManager::LoadMaterial(
                        materialAssetName,
                        shader);
                }

                Material* material =
                    AssetManager::GetMaterial(
                        materialAssetName);

                if (material)
                {
                    // BASE COLOR
                    aiColor3D diffuseColor(
                        1.0f,
                        1.0f,
                        1.0f);

                    if (aiMat->Get(
                            AI_MATKEY_COLOR_DIFFUSE,
                            diffuseColor)
                        == AI_SUCCESS)
                    {
                        material->SetBaseColor(
                            Vector3{
                                diffuseColor.r,
                                diffuseColor.g,
                                diffuseColor.b
                            });
                    }

                    // DIFFUSE TEXTURE
                    aiString texturePath;

                    if (aiMat->GetTexture(
                            aiTextureType_DIFFUSE,
                            0,
                            &texturePath)
                        == AI_SUCCESS)
                    {
                        std::filesystem::path textureFullPath =
                            modelDirectory /
                            texturePath.C_Str();

                        const std::string textureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_Texture";

                        if (AssetManager::LoadTexture(
                                textureName,
                                textureFullPath.string()))
                        {
                            Texture* texture =
                                AssetManager::GetTexture(
                                    textureName);

                            if (texture)
                            {
                                material->SetTexture(
                                    texture);
                            }
                        }
                    }

                    result->SetMaterial(
                        material);

                    std::cout
                        << "Material asignado al mesh: "
                        << materialAssetName
                        << '\n';
                }
            }
            else
            {
                std::cerr
                    << "ERROR: no existe el shader Basic"
                    << std::endl;
            }
        }
    }

    std::cout
        << "Mesh importado: "
        << mesh->mName.C_Str()
        << " | Vertices: "
        << vertices.size()
        << " | Indices: "
        << indices.size()
        << " | UV: "
        << (mesh->mTextureCoords[0]
            ? "YES"
            : "NO")
        << std::endl;

    return result;
}

void ProcessNode(
    aiNode* node,
    const aiScene* scene,
    Model& model,
    const std::filesystem::path& modelDirectory)
{
    if (!node)
        return;

    const aiMatrix4x4& transform =
        node->mTransformation;

    for (unsigned int i = 0;
         i < node->mNumMeshes;
         ++i)
    {
        aiMesh* mesh =
            scene->mMeshes[node->mMeshes[i]];

        auto importedMesh =
            ProcessMesh(
                mesh,
                scene,
                modelDirectory);

        if (importedMesh)
        {
            model.AddMesh(
                std::move(importedMesh));
        }
    }

    for (unsigned int i = 0;
         i < node->mNumChildren;
         ++i)
    {
        ProcessNode(
            node->mChildren[i],
            scene,
            model,
            modelDirectory);
    }
}

}

std::unique_ptr<Model> ModelImporter::Import(
    const std::string& path)
{
    std::filesystem::path modelPath(path);
    std::filesystem::path modelDirectory =
        modelPath.parent_path();
    
    Assimp::Importer importer;

    const aiScene* scene =
        importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_JoinIdenticalVertices);

    if (!scene ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
        !scene->mRootNode)
    {
        std::cerr
            << "Error importando modelo: "
            << path
            << std::endl;

        std::cerr
            << importer.GetErrorString()
            << std::endl;

        return nullptr;
    }

    auto model =
        std::make_unique<Model>();

    ProcessNode(
        scene->mRootNode,
        scene,
        *model,
        modelDirectory);

    if (!model->GetMeshes().empty())
    {
        bool hasBounds = false;

        Vector3 modelMin;
        Vector3 modelMax;

        for (const auto& mesh :
            model->GetMeshes())
        {
            if (!mesh)
                continue;

            const Vector3& meshMin =
                mesh->GetBoundsMin();

            const Vector3& meshMax =
                mesh->GetBoundsMax();

            if (!hasBounds)
            {
                modelMin = meshMin;
                modelMax = meshMax;

                hasBounds = true;
            }
            else
            {
                modelMin.x =
                    std::min(
                        modelMin.x,
                        meshMin.x);

                modelMin.y =
                    std::min(
                        modelMin.y,
                        meshMin.y);

                modelMin.z =
                    std::min(
                        modelMin.z,
                        meshMin.z);

                modelMax.x =
                    std::max(
                        modelMax.x,
                        meshMax.x);

                modelMax.y =
                    std::max(
                        modelMax.y,
                        meshMax.y);

                modelMax.z =
                    std::max(
                        modelMax.z,
                        meshMax.z);
            }
        }

        if (hasBounds)
        {
            model->SetBounds(
                modelMin,
                modelMax);
        }
    }

    std::cout
        << "Modelo importado correctamente: "
        << model->GetMeshes().size()
        << " meshes"
        << std::endl;

    return model;
}

}