#include "camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace gfx
{
    void Camera::ortho(float left, float right, float bottom, float top)
    {
        proj = glm::ortho(left, right, bottom, top);
    }

    auto Camera::vp_matrix() const -> glm::mat4
    {
        return proj * view;
    }
} // namespace gfx
