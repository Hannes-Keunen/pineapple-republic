#include "shader.hpp"

#include "result.hpp"

#include <fmt/printf.h>
#include <glad/glad.h>
#include <spirv_glsl.hpp>

#include <cstdio>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace gfx::gl
{
    template<typename T>
    auto read_file(const std::string& filename) -> Result<std::vector<T>, FileNotFound>
    {
        FILE* file = fopen(filename.data(), "r");
        if (file == nullptr)
        {
            return FileNotFound { filename };
        }

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        size_t count = size / sizeof(T);
        std::vector<T> result(count);
        fread(result.data(), sizeof(T), count, file);
        fclose(file);
        return result;
    }

    static auto load_shader(GLenum type, const std::string& name) -> Result<uint32_t, FileNotFound, ShaderError>
    {
        auto spirv = read_file<uint32_t>(name);
        if (!spirv.is_ok())
        {
            fmt::print("Error loading file {}\n", name);
            return spirv.error_as<FileNotFound>();
        }
        spirv_cross::CompilerGLSL glsl(spirv.unwrap());
        auto source = glsl.compile();

        GLuint shader = glCreateShader(type);
        auto src_ptr = source.data();
        glShaderSource(shader, 1, &src_ptr, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE)
        {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> buffer(length);
            glGetShaderInfoLog(shader, length, &length, buffer.data());
            std::string msg(buffer.begin(), buffer.end());
            return ShaderError { msg };
        }

        return shader;
    }

    static auto build_program(std::unordered_map<GLenum, GLuint> stages) -> Result<uint32_t, ShaderError>
    {
        GLuint program = glCreateProgram();

        for (const auto& [type, shader] : stages)
        {
            glAttachShader(program, shader);
        }
        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success != GL_TRUE)
        {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> buffer(length);
            glGetProgramInfoLog(program, length, &length, buffer.data());
            std::string msg(buffer.begin(), buffer.end());
            return ShaderError { msg };
        }

        for (const auto& [type, shader] : stages)
        {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }
        return program;
    }

    Shader::~Shader()
    {
        if (id != 0)
        {
            glDeleteProgram(id);
        }
    }

    auto Shader::create(const std::string& name) -> Result<Shader, FileNotFound, ShaderError>
    {
        std::unordered_map<GLenum, GLuint> stages;
        auto vertex = load_shader(GL_VERTEX_SHADER, "shaders/" + name + ".vert.spv");
        if (!vertex.is_ok())
        {
            return vertex.propagate();
        }

        auto fragment = load_shader(GL_FRAGMENT_SHADER, "shaders/" + name + ".frag.spv");
        if (!vertex.is_ok())
        {
            return vertex.propagate();
        }

        stages[GL_VERTEX_SHADER] = vertex.unwrap();
        stages[GL_FRAGMENT_SHADER] = fragment.unwrap();
        auto prog = build_program(stages);
        if (!prog.is_ok())
        {
            return prog.propagate();
        }
        return Shader(prog.unwrap());
    }

} // namespace gfx::gl
