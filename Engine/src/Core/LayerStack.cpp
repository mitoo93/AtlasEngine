#include "AtlasEngine/Core/LayerStack.h"
#include "AtlasEngine/Core/Layer.h"

#include <algorithm>
#include <iostream>

namespace AtlasEngine
{

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
    for (auto it = m_Layers.rbegin();
         it != m_Layers.rend();
         ++it)
    {
        (*it)->OnDetach();
        delete *it;
    }

    m_Layers.clear();
}

void LayerStack::PushLayer(
    Layer* layer)
{
    m_Layers.emplace(
        m_Layers.begin() + m_LayerInsertIndex,
        layer);

    ++m_LayerInsertIndex;

    layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay)
{
    std::cout << "LayerStack PushOverlay: "
              << overlay->GetName()
              << '\n';

    m_Layers.emplace_back(overlay);

    overlay->OnAttach();
}

void LayerStack::PopLayer(
    Layer* layer)
{
    auto it =
        std::find(
            m_Layers.begin(),
            m_Layers.begin() + m_LayerInsertIndex,
            layer);

    if (it != m_Layers.end())
    {
        layer->OnDetach();

        m_Layers.erase(it);

        --m_LayerInsertIndex;
    }
}

void LayerStack::PopOverlay(
    Layer* overlay)
{
    auto it =
        std::find(
            m_Layers.begin() + m_LayerInsertIndex,
            m_Layers.end(),
            overlay);

    if (it != m_Layers.end())
    {
        overlay->OnDetach();

        m_Layers.erase(it);
    }
}

}