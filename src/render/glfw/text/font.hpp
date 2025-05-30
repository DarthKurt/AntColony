#include <stb_truetype.h>
#include <glad/glad.h>

namespace AntColony::Render::GLFW::Text
{
    // Cache for common characters
    struct GlyphInfo
    {
        int advance;        // Advance width
        int lsb;            // Left side bearing
        int x0, y0, x1, y1; // Bitmap box
    };

    struct Font
    {
        // Font resource data
        unsigned char *bitmap; // Buffer for glyph bitmaps
        int w, h;              // Bitmap dimensions (e.g., 512x512)
        float size;            // Font size in pixels
        stbtt_fontinfo font;   // stb_truetype font info
        GLuint tex_id;         // OpenGL texture ID for glyphs

        // Font metrics
        float scale;      // Scale for pixel height
        int ascent;       // Font ascent (pixels above baseline)
        int descent;      // Font descent (pixels below baseline)
        int lineGap;      // Line gap between baselines
        float lineHeight; // Total line height (ascent + descent + lineGap)

        // Rendering options
        float spacing; // Additional character spacing
        bool sdf;      // Whether this is a signed distance field font

        GlyphInfo glyphCache[128]; // Cache for ASCII characters
    };
}
