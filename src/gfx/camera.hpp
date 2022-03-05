#pragma once

#include <glm/glm.hpp>

namespace gfx
{
    class Camera
    {
    public:
        void ortho(float left, float right, float bottom, float top);
        auto vp_matrix() const -> glm::mat4;
    private:
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
    };

} // namespace gfx
