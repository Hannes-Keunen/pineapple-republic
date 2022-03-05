#include "texture.hpp"

#include <glad/glad.h>

namespace gfx::gl
{
    Texture::~Texture()
    {
        if (id == 0)
        {
            glDeleteTextures(1, &id);
        }
    }

    static auto get_format(TextureFormat format) -> GLenum
    {
        switch (format)
        {
        case TextureFormat::R8U: return GL_RED;
        case TextureFormat::Rg8U: return GL_RG;
        case TextureFormat::Rgb8U: return GL_RGB;
        case TextureFormat::Rgba8U: return GL_RGBA;
        default: return 0;
        }
    }

    static auto get_internal_format(TextureFormat format) -> GLenum
    {
        switch (format)
        {
        case TextureFormat::R8U: return GL_RED;
        case TextureFormat::Rg8U: return GL_RG;
        case TextureFormat::Rgb8U: return GL_RGB;
        case TextureFormat::Rgba8U: return GL_RGBA;
        default: return 0;
        }
    }

    static auto get_type(TextureFormat format) -> GLenum
    {
        switch (format)
        {
        case TextureFormat::R8U:
        case TextureFormat::Rg8U:
        case TextureFormat::Rgb8U:
        case TextureFormat::Rgba8U:
            return GL_UNSIGNED_BYTE;
        default: return 0;
        }
    }

    auto Texture::create(int width, int height, TextureFormat format, const void* data) -> std::optional<Texture>
    {
        GLuint id;
        glGenTextures(1, &id);
        if (id == 0)
        {
            return std::nullopt;
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, get_internal_format(format), width, height, 0, get_internal_format(format), get_type(format), data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return Texture(id);
    }
    
} // namespace gfx::gl
