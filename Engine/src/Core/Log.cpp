#include "AtlasEngine/Core/Log.h"

#include <iostream>

namespace AtlasEngine
{

void Log::Info(const std::string& message)
{
    std::cout << "[Info] " << message << std::endl;
}

}