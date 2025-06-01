#pragma once

#include "_fwd.hpp"
#include <memory>

namespace AntColony::Render
{
    class FrameContext
    {
    public:
        virtual void onBeforeRender() = 0;
        virtual void onAfterRender() = 0;
        virtual const std::shared_ptr<Renderer> getRenderer() const = 0;
        virtual ~FrameContext() {}
    };
}
