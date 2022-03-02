#pragma once

#include <result.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace gfx::gl
{
    struct FileNotFound { std::string filename; };
    struct ShaderError { std::string message; };

    class Shader
    {
    public:
        Shader(): id(0) {}
        Shader(uint32_t id) : id(id) {}
        Shader(const Shader&) = delete;
        Shader(Shader&& other) { *this = std::forward<Shader>(other); }
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&& other)
        {
            id = other.id;
            other.id = 0;
            return *this;
        }

        ~Shader();

        constexpr uint32_t gl_id() const
        {
            return id;
        }

    public:
        static auto create(const std::string& name) -> Result<Shader, FileNotFound, ShaderError>;
    private:
        uint32_t id;
    };

} // namespace gfx::gl
