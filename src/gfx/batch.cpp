#include "batch.hpp"

#include <vector>

namespace gfx
{
    void Batch::begin()
    {
        mapped_buffer = vbo.map_as<Vertex>();
        size = 0;
    }

    void Batch::submit(float x, float y, float w, float h)
    {
        if (size == capacity)
        {
            flush();
            begin();
        }

        mapped_buffer[4 * size + 0] = {{x    , y + h}, {0.0f, 1.0f}}; // top left
        mapped_buffer[4 * size + 1] = {{x + w, y + h}, {1.0f, 1.0f}}; // top right
        mapped_buffer[4 * size + 2] = {{x    , y    }, {0.0f, 0.0f}}; // bottom left
        mapped_buffer[4 * size + 3] = {{x + w, y    }, {1.0f, 0.0f}}; // bottom right
        size += 1;
    }

    void Batch::flush()
    {
        vbo.unmap();

        glUseProgram(shader.gl_id());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.gl_id());
        glBindVertexArray(vao.gl_id());
        glDrawElements(GL_TRIANGLES, 6 * size, GL_UNSIGNED_SHORT, 0);
    }

    static auto create_indices(int count) -> std::vector<uint16_t>
    {
        std::vector<uint16_t> indices;
        indices.reserve(6 * count);
        for (int i = 0; i < count; i++)
        {
            indices.push_back(i * count + 0);
            indices.push_back(i * count + 1);
            indices.push_back(i * count + 2);
            indices.push_back(i * count + 2);
            indices.push_back(i * count + 1);
            indices.push_back(i * count + 3);
        }
        return indices;
    }

    auto Batch::create(int capacity) -> std::optional<Batch>
    {
        auto shader = gl::Shader::create("simple");
        if (!shader.is_ok())
        {
            return std::nullopt;
        }

        auto vbo = gl::Buffer::empty(4 * capacity * sizeof(Vertex));
        if (vbo == std::nullopt)
        {
            return std::nullopt;
        }

        auto vao = gl::VertexArray::create();
        if (vao == std::nullopt)
        {
            return std::nullopt;
        }
        vao.value().with_vertex_buffer<Vertex>(vbo.value());

        auto ibo = gl::Buffer::from_array(create_indices(capacity));
        if (ibo == std::nullopt)
        {
            return std::nullopt;
        }

        return Batch(1, shader.unwrap(), std::move(vao.value()), std::move(vbo.value()), std::move(ibo.value()));
    }

} // namespace gfx
