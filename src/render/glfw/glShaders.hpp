namespace AntColony::Render::GLFW
{
    // Vertex shader source used for all drawing
    constexpr const char *VERTEXT_SHADER_SOURCE =
        R"(
            #version 330 core
            layout(location = 0) in vec2 position;
            uniform vec2 uCenter;
            uniform float uScale;
            void main() {
                gl_Position = vec4((position * uScale) + uCenter, 0.0, 1.0);
            }
    )";

    // Fragment shader source used for all drawing
    constexpr const char *FRAGMENT_SHADER_SRC =
        R"(
            #version 330 core
            out vec4 fragColor;
            uniform vec3 uColor;
            void main() {
                fragColor = vec4(uColor, 1.0);
            }
        )";
}