#pragma once

#include <string>

namespace AtlasEngine
{

class Texture
{
public:

    Texture();

    ~Texture();

    Texture(
        const Texture&) = delete;

    Texture& operator=(
        const Texture&) = delete;

    Texture(
        Texture&&) = delete;

    Texture& operator=(
        Texture&&) = delete;

    bool Load(
        const std::string& path);

    void Bind(
        unsigned int slot = 0) const;

    void Unbind() const;

    int GetWidth() const;
    int GetHeight() const;

    void SetName(
        const std::string& name);

    const std::string& GetName() const;

private:

    unsigned int m_RendererID = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_Channels = 0;

    std::string m_Name;
};

}