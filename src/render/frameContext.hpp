#pragma once

namespace AntColony::Render
{
    class FrameContext
    {
    public:
        virtual void onBeforeRender() = 0;
        virtual void onAfterRender() = 0;
        virtual ~FrameContext() {}
    };
}
