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

Texture* LoadMaterialTexture(
    const aiScene* scene,
    const aiString& texturePath,
    const std::filesystem::path& modelDirectory,
    const std::string& textureAssetName)
{
    if (!scene)
        return nullptr;

    const std::string path =
        texturePath.C_Str();

    // ----------------------------------------------------
    // EMBEDDED TEXTURE
    // ----------------------------------------------------

    if (!path.empty() &&
        path[0] == '*')
    {
        unsigned int textureIndex = 0;

        try
        {
            textureIndex =
                static_cast<unsigned int>(
                    std::stoul(
                        path.substr(1)));
        }
        catch (...)
        {
            return nullptr;
        }

        if (textureIndex >=
            scene->mNumTextures)
        {
            return nullptr;
        }

        const aiTexture* embeddedTexture =
            scene->mTextures[
                textureIndex];

        if (!embeddedTexture)
            return nullptr;

        // GLB normalmente guarda PNG/JPG comprimido.
        // En ese caso mHeight == 0 y mWidth es el tamaño
        // del bloque comprimido en bytes.
        if (embeddedTexture->mHeight == 0)
        {
            const unsigned char* data =
                reinterpret_cast<
                    const unsigned char*>(
                        embeddedTexture->pcData);

            const int size =
                static_cast<int>(
                    embeddedTexture->mWidth);

            if (!AssetManager::
                    LoadTextureFromMemory(
                        textureAssetName,
                        data,
                        size))
            {
                return nullptr;
            }

            std::cout
                << "Textura embebida cargada: "
                << path
                << " -> "
                << textureAssetName
                << '\n';

            return AssetManager::GetTexture(
                textureAssetName);
        }

        // Por ahora no tratamos aiTexture sin comprimir.
        std::cerr
            << "AVISO: textura embebida sin comprimir no soportada: "
            << path
            << '\n';

        return nullptr;
    }

    // ----------------------------------------------------
    // EXTERNAL TEXTURE
    // ----------------------------------------------------

    std::filesystem::path fullPath =
        modelDirectory /
        path;

    if (!AssetManager::LoadTexture(
            textureAssetName,
            fullPath.string()))
    {
        return nullptr;
    }

    return AssetManager::GetTexture(
        textureAssetName);
}

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

        if (mesh->HasTangentsAndBitangents())
        {
            vertex.Tangent[0] =
                mesh->mTangents[i].x;

            vertex.Tangent[1] =
                mesh->mTangents[i].y;

            vertex.Tangent[2] =
                mesh->mTangents[i].z;

            vertex.Bitangent[0] =
                mesh->mBitangents[i].x;

            vertex.Bitangent[1] =
                mesh->mBitangents[i].y;

            vertex.Bitangent[2] =
                mesh->mBitangents[i].z;
        }
        else
        {
            vertex.Tangent[0] = 0.0f;
            vertex.Tangent[1] = 0.0f;
            vertex.Tangent[2] = 0.0f;

            vertex.Bitangent[0] = 0.0f;
            vertex.Bitangent[1] = 0.0f;
            vertex.Bitangent[2] = 0.0f;
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

            // ----------------------------------------------------
            // DETECT MATERIAL TYPE
            // ----------------------------------------------------

            bool isPBRMaterial =
                aiMat->GetTextureCount(
                    aiTextureType_METALNESS) > 0 ||
                aiMat->GetTextureCount(
                    aiTextureType_DIFFUSE_ROUGHNESS) > 0 ||
                aiMat->GetTextureCount(
                    aiTextureType_AMBIENT_OCCLUSION) > 0;


            // ----------------------------------------------------
            // SELECT SHADER
            // ----------------------------------------------------

            Shader* shader =
                AssetManager::GetShader(
                    isPBRMaterial
                        ? "PBR"
                        : "Basic");
            std::cout
                << "Material "
                << materialName.C_Str()
                << " detectado como: "
                << (isPBRMaterial
                    ? "PBR"
                    : "Basic")
                << '\n';


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
                    // --------------------------------------------
                    // PBR SCALAR VALUES
                    // --------------------------------------------

                    float metallic =
                        0.0f;

                    if (aiMat->Get(
                            AI_MATKEY_METALLIC_FACTOR,
                            metallic)
                        == AI_SUCCESS)
                    {
                        material->SetMetallic(
                            metallic);
                    }


                    float roughness =
                        0.5f;

                    if (aiMat->Get(
                            AI_MATKEY_ROUGHNESS_FACTOR,
                            roughness)
                        == AI_SUCCESS)
                    {
                        material->SetRoughness(
                            roughness);
                    }


                    // --------------------------------------------
                    // BASE COLOR
                    // --------------------------------------------
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

                    // --------------------------------------------
                    // DIFFUSE / ALBEDO TEXTURE
                    // --------------------------------------------

                    aiString texturePath;

                    if (aiMat->GetTexture(
                            aiTextureType_DIFFUSE,
                            0,
                            &texturePath)
                        == AI_SUCCESS)
                    {
                        const std::string textureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_Texture";

                        Texture* texture =
                            LoadMaterialTexture(
                                scene,
                                texturePath,
                                modelDirectory,
                                textureName);

                        if (texture)
                        {
                            material->SetTexture(
                                texture);

                            std::cout
                                << "Albedo/Diffuse map asignado al material: "
                                << textureName
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // NORMAL TEXTURE
                    // --------------------------------------------

                    aiString normalTexturePath;

                    bool hasNormalTexture =
                        aiMat->GetTexture(
                            aiTextureType_NORMALS,
                            0,
                            &normalTexturePath)
                        == AI_SUCCESS;

                    if (!hasNormalTexture)
                    {
                        hasNormalTexture =
                            aiMat->GetTexture(
                                aiTextureType_HEIGHT,
                                0,
                                &normalTexturePath)
                            == AI_SUCCESS;

                        if (hasNormalTexture)
                        {
                            std::cout
                                << "Normal map detectado como HEIGHT/map_Bump: "
                                << normalTexturePath.C_Str()
                                << '\n';
                        }
                    }

                    if (hasNormalTexture)
                    {
                        const std::string normalTextureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_NormalTexture";

                        Texture* normalTexture =
                            LoadMaterialTexture(
                                scene,
                                normalTexturePath,
                                modelDirectory,
                                normalTextureName);

                        if (normalTexture)
                        {
                            material->SetNormalTexture(
                                normalTexture);

                            std::cout
                                << "Normal map asignado al material: "
                                << normalTextureName
                                << '\n';
                        }
                        else
                        {
                            std::cerr
                                << "ERROR cargando normal map: "
                                << normalTexturePath.C_Str()
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // SPECULAR TEXTURE
                    // --------------------------------------------

                    aiString specularTexturePath;

                    if (aiMat->GetTexture(
                            aiTextureType_SPECULAR,
                            0,
                            &specularTexturePath)
                        == AI_SUCCESS)
                    {
                        std::cout
                            << "Specular map detectado: "
                            << specularTexturePath.C_Str()
                            << '\n';

                        std::filesystem::path specularFullPath =
                            modelDirectory /
                            specularTexturePath.C_Str();

                        const std::string specularTextureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_SpecularTexture";

                        if (AssetManager::LoadTexture(
                                specularTextureName,
                                specularFullPath.string()))
                        {
                            Texture* specularTexture =
                                AssetManager::GetTexture(
                                    specularTextureName);

                            if (specularTexture)
                            {
                                material->SetSpecularTexture(
                                    specularTexture);

                                std::cout
                                    << "Specular map asignado al material: "
                                    << specularTextureName
                                    << '\n';
                            }
                        }
                        else
                        {
                            std::cerr
                                << "ERROR cargando specular map: "
                                << specularFullPath.string()
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // METALLIC TEXTURE
                    // --------------------------------------------

                    aiString metallicTexturePath;

                    if (aiMat->GetTexture(
                            aiTextureType_METALNESS,
                            0,
                            &metallicTexturePath)
                        == AI_SUCCESS)
                    {
                        const std::string metallicTextureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_MetallicTexture";

                        Texture* metallicTexture =
                            LoadMaterialTexture(
                                scene,
                                metallicTexturePath,
                                modelDirectory,
                                metallicTextureName);

                        if (metallicTexture)
                        {
                            material->SetMetallicTexture(
                                metallicTexture);

                            std::cout
                                << "Metallic map asignado al material: "
                                << metallicTextureName
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // ROUGHNESS TEXTURE
                    // --------------------------------------------

                    aiString roughnessTexturePath;

                    if (aiMat->GetTexture(
                            aiTextureType_DIFFUSE_ROUGHNESS,
                            0,
                            &roughnessTexturePath)
                        == AI_SUCCESS)
                    {
                        const std::string roughnessTextureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_RoughnessTexture";

                        Texture* roughnessTexture =
                            LoadMaterialTexture(
                                scene,
                                roughnessTexturePath,
                                modelDirectory,
                                roughnessTextureName);

                        if (roughnessTexture)
                        {
                            material->SetRoughnessTexture(
                                roughnessTexture);

                            std::cout
                                << "Roughness map asignado al material: "
                                << roughnessTextureName
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // PACKED METALLIC ROUGHNESS TEXTURE
                    // --------------------------------------------

                    if (metallicTexturePath.length > 0 &&
                        roughnessTexturePath.length > 0)
                    {
                        const std::string metallicPath =
                            metallicTexturePath.C_Str();

                        const std::string roughnessPath =
                            roughnessTexturePath.C_Str();

                        if (metallicPath ==
                            roughnessPath)
                        {
                            material->SetPackedMetallicRoughness(
                                true);

                            std::cout
                                << "Metallic/Roughness empaquetados detectados: "
                                << metallicPath
                                << '\n';
                        }
                    }

                    // --------------------------------------------
                    // AO TEXTURE
                    // --------------------------------------------

                    aiString aoTexturePath;

                    bool hasAOTexture =
                        aiMat->GetTexture(
                            aiTextureType_AMBIENT_OCCLUSION,
                            0,
                            &aoTexturePath)
                        == AI_SUCCESS;

                    if (!hasAOTexture)
                    {
                        hasAOTexture =
                            aiMat->GetTexture(
                                aiTextureType_LIGHTMAP,
                                0,
                                &aoTexturePath)
                            == AI_SUCCESS;

                        if (hasAOTexture)
                        {
                            std::cout
                                << "AO map detectado como LIGHTMAP: "
                                << aoTexturePath.C_Str()
                                << '\n';
                        }
                    }

                    if (hasAOTexture)
                    {
                        const std::string aoTextureName =
                            std::string("Model_") +
                            materialName.C_Str() +
                            "_AOTexture";

                        Texture* aoTexture =
                            LoadMaterialTexture(
                                scene,
                                aoTexturePath,
                                modelDirectory,
                                aoTextureName);

                        if (aoTexture)
                        {
                            material->SetAOTexture(
                                aoTexture);

                            std::cout
                                << "AO map asignado al material: "
                                << aoTextureName
                                << '\n';
                        }
                        else
                        {
                            std::cerr
                                << "ERROR cargando AO map: "
                                << aoTexturePath.C_Str()
                                << '\n';
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
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);

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