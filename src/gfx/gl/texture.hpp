#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gfx::gl
{
    enum class TextureFormat
    {
        R8U,
        Rg8U,
        Rgb8U,
        Rgba8U,
    };

    class Texture
    {
    public:
        Texture(): id(0) {}
        Texture(uint32_t id) : id(id) {}
        Texture(const Texture&) = delete;
        Texture(Texture&& other) { *this = std::forward<Texture>(other); }
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&& other)
        {
            id = other.id;
            other.id = 0;
            return *this;
        }

        ~Texture();

        constexpr auto gl_id() const { return id; }
    public:
        static auto create(int width, int height, TextureFormat format, const void* data) -> std::optional<Texture>;
        static auto empty(int width, int height, TextureFormat format) -> std::optional<Texture>;

    private:
        uint32_t id;
    };

} // namespace gfx::gl
