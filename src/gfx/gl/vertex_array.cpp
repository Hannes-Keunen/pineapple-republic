#include "vertex_array.hpp"

#include "buffer.hpp"

#include <glad/glad.h>

namespace gfx::gl
{
    VertexArray::~VertexArray()
    {
        if (id != 0)
        {
            glDeleteVertexArrays(1, &id);
        }
    }

    void VertexArray::with_vertex_buffer(const Buffer& vbo, VertexLayout layout)
    {
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.gl_id());

        int offset = 0;
        int attr_idx = 0;
        for (const auto& attrib : layout.attribs)
        {
            glEnableVertexAttribArray(attr_idx);
            glVertexAttribPointer(
                attr_idx, attrib.count, attrib.type,
                attrib.normalized, layout.stride,
                reinterpret_cast<const void*>(offset));
            attr_idx++;
            offset += attrib.size;
        }
    }

    auto VertexArray::create() -> std::optional<VertexArray>
    {
        GLuint id;
        glGenVertexArrays(1, &id);
        if (id == 0)
        {
            return std::nullopt;
        }
        return VertexArray(id);
    }

} // namespace gfx::gl
