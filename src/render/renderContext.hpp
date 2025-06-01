#pragma once

#include "_fwd.hpp"
#include <memory>

namespace AntColony::Render
{
    class RenderContext
    {
    public:
        virtual ~RenderContext() {}
        virtual void init() = 0;
        virtual bool getInited() const = 0;
        virtual bool shouldClose() const = 0;
        virtual std::unique_ptr<FrameContext> getFrameContext() const = 0;
    };
}
