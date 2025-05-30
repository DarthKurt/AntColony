#pragma once

#include "../core/logger.hpp"
#include "renderEngines.hpp"
#include "renderContext.hpp"

#include <memory>

namespace AntColony::Render
{
    std::unique_ptr<RenderContext> initRenderContext(RENDER_ENGINE engine, std::shared_ptr<Core::Logger> logger);
}