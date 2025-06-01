#pragma once
#include "_fwd.hpp"
#include "../../core/_fwd.hpp"

#include "../renderContext.hpp"

#include <memory>
#include "_gl.hpp"

namespace AntColony::Render::GLFW
{
    class GLRenderContext : public RenderContext
    {
    public:

        static std::unique_ptr<RenderContext> getInstance(const std::shared_ptr<Core::Logger> logger);

        ~GLRenderContext() override;

        // RenderContext
        void init() override;
        bool getInited() const override;
        bool shouldClose() const override;
        std::unique_ptr<FrameContext> getFrameContext() const override;

    private:
        explicit GLRenderContext(const std::shared_ptr<Core::Logger> logger);
        const std::shared_ptr<Core::Logger> logger;

        bool isInited;
        GLFWwindow *window;
        const std::shared_ptr<GLRenderer> renderer;

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    };
}
