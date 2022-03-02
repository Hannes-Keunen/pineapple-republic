#pragma once

#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "gl/vertex_array.hpp"
#include "vertex.hpp"

#include <optional>

namespace gfx
{
    class Batch
    {
        struct Vertex
        {
            float pos[2];
            float uv[2];

            static auto layout() -> std::vector<VertexAttributeInfo>
            {
                return {
                    attrib<decltype(Vertex::pos)>(),
                    attrib<decltype(Vertex::uv)>()
                };
            }
        };
    private:
        Batch(int capacity, gl::Shader&& shader, gl::VertexArray&& vao, gl::Buffer&& vbo, gl::Buffer&& ibo)
            : size(0), capacity(capacity)
            , shader(std::forward<gl::Shader>(shader))
            , vao(std::forward<gl::VertexArray>(vao))
            , vbo(std::forward<gl::Buffer>(vbo))
            , ibo(std::forward<gl::Buffer>(ibo)) {}
    public:
        Batch(const Batch&) = delete;
        Batch(Batch&&) = default;
        Batch& operator=(const Batch&) = delete;
        Batch& operator=(Batch&&) = default;

        void begin();
        void submit(float x, float y, float w, float h);
        void flush();
    public:
        static auto create(int capacity) -> std::optional<Batch>;
    private:
        int size;
        int capacity;
        Vertex* mapped_buffer;

        gl::Shader shader;
        gl::VertexArray vao;
        gl::Buffer vbo, ibo;
    };

} // namespace gfx
