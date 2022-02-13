#pragma once

#include <imgui.h>

struct GLFWwindow;

namespace imgui
{
    void init(GLFWwindow* window);
    void shutdown();
    void begin();
    void draw();
} // namespace imgui
