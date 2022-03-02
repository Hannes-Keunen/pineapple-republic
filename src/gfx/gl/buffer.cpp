#include "buffer.hpp"

#include <glad/glad.h>

namespace gfx::gl
{

    Buffer::~Buffer()
    {
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
        }
    }

    auto Buffer::map() -> void*
    {
        return glMapNamedBuffer(id, GL_WRITE_ONLY);
    }

    void Buffer::unmap()
    {
        glUnmapNamedBuffer(id);
    }

    auto Buffer::from_raw(const void* data, std::size_t size) -> std::optional<Buffer>
    {
        GLuint buffer;
        glCreateBuffers(1, &buffer);
        if (buffer == 0)
        {
            return std::nullopt;
        }

        glNamedBufferData(buffer, size, data, GL_STATIC_DRAW);
        return Buffer(buffer);
    }

    auto Buffer::empty(std::size_t size) -> std::optional<Buffer>
    {
        GLuint buffer;
        glCreateBuffers(1, &buffer);
        if (buffer == 0)
        {
            return std::nullopt;
        }

        glNamedBufferData(buffer, size, nullptr, GL_DYNAMIC_DRAW);
        return Buffer(buffer);
    }

} // namespace gfx::gl
