#include "AtlasEngine/Core/Time.h"

namespace AtlasEngine
{

float Time::s_Time = 0.0f;

float Time::s_DeltaTime = 0.0f;

float Time::s_LastFrameTime = 0.0f;

void Time::Update(float currentTime)
{
    s_Time = currentTime;

    s_DeltaTime = currentTime - s_LastFrameTime;

    s_LastFrameTime = currentTime;
}

float Time::GetTime()
{
    return s_Time;
}

float Time::GetDeltaTime()
{
    return s_DeltaTime;
}

}