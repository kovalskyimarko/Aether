#pragma once

namespace Aether
{
    class Time
    {
    public:
        void Init();
        void Update();

        float GetDeltaTime() const;
        double GetTime() const;
        float GetTimeF() const;

        void SetMaxDeltaTime(float maxDelta) { maxDeltaTime = maxDelta; }

    private:
        float deltaTime = 0.0f;
        double lastFrameTime = 0.0;
        float maxDeltaTime = 0.1f;
    };
}