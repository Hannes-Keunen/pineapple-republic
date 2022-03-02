#pragma once

#include "gfx/vertex.hpp"

#include <cstdint>
#include <optional>
#include <utility>

namespace gfx::gl
{
    class Buffer;

    class VertexArray
    {
    public:
        VertexArray(): id(0) {}
        VertexArray(uint32_t id) : id(id) {}
        VertexArray(const VertexArray&) = delete;
        VertexArray(VertexArray&& other) { *this = std::forward<VertexArray>(other); }
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& other)
        {
            id = other.id;
            other.id = 0;
            return *this;
        }

        ~VertexArray();

        template <Vertex V>
        void with_vertex_buffer(const Buffer& vbo)
        {
            with_vertex_buffer(vbo, VertexLayout::from<V>());
        }

        void with_vertex_buffer(const Buffer& vbo, VertexLayout layout);

        auto gl_id() const { return id; }

    public:
        static auto create() -> std::optional<VertexArray>;

    private:
        uint32_t id;
    };

} // namespace gfx::gl
