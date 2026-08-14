#pragma once

#include <filesystem>
#include <string>

namespace AtlasEngine
{

class AssetBrowser
{
public:

    AssetBrowser();

    void OnImGuiRender();

private:

    void DrawBreadcrumb();

    const char* GetFileTypeLabel(
        const std::filesystem::path& path) const;

private:

    std::filesystem::path m_RootPath;
    std::filesystem::path m_CurrentPath;
};

}