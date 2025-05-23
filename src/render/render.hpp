#pragma once

#include "renderEngines.hpp"
#include "renderContext.hpp"

#include <memory>

namespace AntColony::Render
{
    std::unique_ptr<RenderContext> initRenderContext(RENDER_ENGINE engine);
}