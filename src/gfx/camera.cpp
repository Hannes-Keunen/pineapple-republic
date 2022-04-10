#include "camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace gfx
{
    void Camera::ortho(float left, float right, float bottom, float top)
    {
        proj = glm::ortho(left, right, bottom, top);
    }

    void Camera::translate(const glm::vec2& delta)
    {
        view = glm::translate(view, glm::vec3(delta, 0.0f));
    }

    void Camera::scale(const float factor)
    {
        view = glm::scale(view, glm::vec3(factor, factor, factor));
    }

    auto Camera::vp_matrix() const -> glm::mat4
    {
        return proj * view;
    }
} // namespace gfx
