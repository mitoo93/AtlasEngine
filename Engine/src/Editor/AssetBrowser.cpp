#include "AtlasEngine/Editor/AssetBrowser.h"

#include <imgui.h>

namespace AtlasEngine
{

AssetBrowser::AssetBrowser()
    :
    m_RootPath("Assets"),
    m_CurrentPath(m_RootPath)
{
}

void AssetBrowser::OnImGuiRender()
{
    if (!std::filesystem::exists(m_RootPath))
    {
        ImGui::Begin("Asset Browser");

        ImGui::Text(
            "No se encontro la carpeta Assets");

        ImGui::End();

        return;
    }

    ImGui::Begin("Asset Browser");

    // ----------------------------------------------------
    // BACK
    // ----------------------------------------------------

    if (m_CurrentPath != m_RootPath)
    {
        if (ImGui::Button("<-"))
        {
            m_CurrentPath =
                m_CurrentPath.parent_path();
        }

        ImGui::SameLine();
    }

    DrawBreadcrumb();
    
    ImGui::Separator();

    // ----------------------------------------------------
    // CONTENT
    // ----------------------------------------------------

    for (const auto& entry :
         std::filesystem::directory_iterator(
             m_CurrentPath))
    {
        const auto path =
            entry.path();

        const std::string filename =
            path.filename().string();

        if (entry.is_directory())
        {
            if (ImGui::Selectable(
                    ("[DIR] " + filename).c_str()))
            {
                m_CurrentPath =
                    path;
            }
        }
        else
        {
            ImGui::Selectable(
                (std::string(GetFileTypeLabel(path)) +
                " " +
                filename).c_str());

            if (ImGui::BeginDragDropSource())
            {
                std::string assetPath =
                    path.string();

                ImGui::SetDragDropPayload(
                    "ASSET_PATH",
                    assetPath.c_str(),
                    assetPath.size() + 1);

                ImGui::Text(
                    "%s",
                    filename.c_str());

                ImGui::EndDragDropSource();
            }
        }
    }

    ImGui::End();
}

const char* AssetBrowser::GetFileTypeLabel(
    const std::filesystem::path& path) const
{
    const std::string extension =
        path.extension().string();

    if (extension == ".vert")
        return "[VERT]";

    if (extension == ".frag")
        return "[FRAG]";

    if (extension == ".scene")
        return "[SCENE]";

    if (extension == ".obj")
        return "[OBJ]";

    if (extension == ".gltf")
        return "[GLTF]";

    if (extension == ".glb")
        return "[GLB]";

    if (extension == ".png")
        return "[PNG]";

    if (extension == ".jpg" ||
        extension == ".jpeg")
    {
        return "[IMAGE]";
    }

    return "[FILE]";
}

void AssetBrowser::DrawBreadcrumb()
{
    std::filesystem::path relativePath =
        std::filesystem::relative(
            m_CurrentPath,
            m_RootPath);

    std::filesystem::path current;

    if (ImGui::Button("Assets"))
    {
        m_CurrentPath =
            m_RootPath;
    }

    if (relativePath == ".")
    {
        return;
    }

    for (const auto& part : relativePath)
    {
        current /= part;

        ImGui::SameLine();
        ImGui::Text(">");

        ImGui::SameLine();

        if (ImGui::Button(
                part.string().c_str()))
        {
            m_CurrentPath =
                m_RootPath / current;
        }
    }
}

}