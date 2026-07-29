#pragma once

namespace AtlasEngine
{

class Time
{
public:

    static void Update(float currentTime);

    static float GetTime();

    static float GetDeltaTime();

private:

    static float s_Time;

    static float s_DeltaTime;

    static float s_LastFrameTime;
};

}