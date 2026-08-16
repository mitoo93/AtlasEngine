#pragma once

#include "AtlasEngine/Scene/Component.h"

namespace AtlasEngine
{

class Model;

class ModelRendererComponent : public Component
{
public:

    void SetModel(
        Model* model)
    {
        m_Model = model;
    }

    Model* GetModel()
    {
        return m_Model;
    }

    const Model* GetModel() const
    {
        return m_Model;
    }

private:

    Model* m_Model = nullptr;
};

}