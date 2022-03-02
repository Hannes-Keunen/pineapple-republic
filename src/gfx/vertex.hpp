#pragma once

#include <glad/glad.h>

#include <cstdint>
#include <vector>

namespace gfx
{
    struct VertexAttributeInfo
    {
        int count;
        int size;
        GLenum type;
        bool normalized;
    };

    template <typename T> constexpr auto gl_type() -> GLenum;
    template <> constexpr auto gl_type<float>() -> GLenum { return GL_FLOAT; }
    template <> constexpr auto gl_type<uint8_t>() -> GLenum { return GL_UNSIGNED_BYTE; }

    template <typename T> constexpr auto gl_normalize_type() -> bool;
    template <> constexpr auto gl_normalize_type<float>() -> bool { return false; };
    template <> constexpr auto gl_normalize_type<uint8_t>() -> bool { return true; };

    template <typename T>
    constexpr auto make_attrib(int count, bool normalized) -> VertexAttributeInfo
    {
        return VertexAttributeInfo { count, count * static_cast<int>(sizeof(T)), gl_type<T>(), normalized };
    }

    template <typename T>
    constexpr auto make_attrib(int count) -> VertexAttributeInfo
    {
        return make_attrib<T>(count, gl_normalize_type<T>());
    }

    template <typename T> constexpr auto attrib() -> VertexAttributeInfo;
    template <> constexpr auto attrib<float[2]>() -> VertexAttributeInfo { return make_attrib<float>(2); }
    template <> constexpr auto attrib<float[3]>() -> VertexAttributeInfo { return make_attrib<float>(2); }
    template <> constexpr auto attrib<float[4]>() -> VertexAttributeInfo { return make_attrib<float>(2); }

    template <typename T>
    auto layout() -> std::vector<VertexAttributeInfo>;

    template <typename T>
    concept Vertex = requires
    {
        { T::layout() } -> std::same_as<std::vector<VertexAttributeInfo>>;
    };

    struct VertexLayout
    {
        uint32_t stride;
        std::vector<VertexAttributeInfo> attribs;

        template <Vertex V>
        static VertexLayout from()
        {
            auto result = VertexLayout {
                .stride = 0,
                .attribs = V::layout(),
            };
            for (const auto & attr : result.attribs)
            {
                result.stride += attr.size;
            }
            return result;
        }
    };

} // namespace gfx
