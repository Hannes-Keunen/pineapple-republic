#include "gfx/sys/systems.hpp"

#include "game_state.hpp"
#include "gfx/camera.hpp"
#include "log.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <optional>

namespace gfx::sys
{
    void cam_control(GameState& state)
    {
        static std::optional<glm::dvec2> prev;

        auto& window = state.get<GLFWwindow*>();
        auto& camera = state.get<gfx::Camera>();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            if (prev.has_value())
            {
                auto delta = glm::dvec2(x, y) - prev.value();
                camera.translate(delta / 100.0);
            }

            prev = glm::dvec2(x, y);
        }
        else
        {
            prev = std::nullopt;
        }
    }

} // namespace gfx::sys