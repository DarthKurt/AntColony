namespace AntColony::Render::GLFW
{
    const char *VERTEXT_SHADER_SOURCE = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        uniform mat4 uOrtho;
        uniform mat4 uModel = mat4(1.0);  // Default to identity if not provided
        void main() {
            gl_Position = uOrtho * uModel * vec4(aPos, 0.0, 1.0);
        }
    )";

    const char *FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )";

    const char *TEXT_VERTEX_SHADER_SOURCE = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        uniform mat4 uOrtho;
        void main() {
            gl_Position = uOrtho * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char *TEXT_FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D uFontTex;
        uniform vec3 uColor;
        void main() {
            float alpha = texture(uFontTex, TexCoord).r;
            FragColor = vec4(uColor, alpha);
        }
    )";
}