#pragma once

#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "gl/vertex_array.hpp"
#include "vertex.hpp"

#include <glm/glm.hpp>

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
    public:
        struct Stats
        {
            int sprites;
            int draws;
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

        void begin(const glm::mat4& vpmatrix);
        void submit(float x, float y, float w, float h);
        void end();

        constexpr auto& get_stats() const { return stats; }
    public:
        static auto create(int capacity) -> std::optional<Batch>;
    private:
        void reset();
        void flush();
    private:
        int size;
        int capacity;
        glm::mat4 vp_matrix;
        Vertex* mapped_buffer;

        gl::Shader shader;
        gl::VertexArray vao;
        gl::Buffer vbo, ibo;
        
        Stats stats;
    };

} // namespace gfx
