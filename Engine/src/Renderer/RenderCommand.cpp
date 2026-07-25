#include "AtlasEngine/Renderer/RenderCommand.h"

#include <glad/glad.h>

namespace AtlasEngine
{

void RenderCommand::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void RenderCommand::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

}