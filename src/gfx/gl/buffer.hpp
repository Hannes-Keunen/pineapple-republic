#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace gfx::gl
{
    class Buffer
    {
    public:
        Buffer(): id(0) {}
        Buffer(uint32_t id) : id(id) {}
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&& other) { *this = std::forward<Buffer>(other); }
        Buffer& operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&& other)
        {
            id = other.id;
            other.id = 0;
            return *this;
        }

        ~Buffer();

        auto map() -> void*;
        void unmap();

        template <typename T>
        auto map_as() { return reinterpret_cast<T*>(map()); }

        constexpr uint32_t gl_id() const
        {
            return id;
        }

    public:
        static auto from_raw(const void* data, std::size_t size) -> std::optional<Buffer>;
        static auto empty(std::size_t size) -> std::optional<Buffer>;

        template <typename T>
        static auto from_array(std::vector<T> data) -> std::optional<Buffer>
        {
            return from_raw(data.data(), data.size() * sizeof(T));
        }

    private:
        uint32_t id;
    };

} // namespace gl
