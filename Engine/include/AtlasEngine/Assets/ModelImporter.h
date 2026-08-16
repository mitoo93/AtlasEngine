#pragma once

#include <memory>
#include <string>

namespace AtlasEngine
{

class Model;

class ModelImporter
{
public:

    static std::unique_ptr<Model> Import(
        const std::string& path);
};

}