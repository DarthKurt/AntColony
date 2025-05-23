#pragma once

#include "renderer.hpp"
#include <memory>

namespace AntColony::Render
{
    class FrameContext
    {
    public:
        virtual void onBeforeRender() = 0;
        virtual void onAfterRender() = 0;
        virtual const Renderer &getRenderer() const = 0;
        virtual ~FrameContext() {}
    };
}
