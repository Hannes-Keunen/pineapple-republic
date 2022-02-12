#include <entt/entt.hpp>
#include <fmt/printf.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void glfw_error_callback(int code, const char* msg)
{
    fmt::print("[GLFW] error {:d}: {:s}\n", code, msg);
}

void graphics_main(entt::registry* registry)
{
    fmt::print("hallo\n");
    if (!glfwInit())
    {
        fmt::print("glfwInit failed\n");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    auto window = glfwCreateWindow(1920, 1080, "Pineapple Republic", nullptr, nullptr);
    if (window == nullptr)
    {
        fmt::print("failed to create a window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0)
    {
        fmt::print("gladLoadGLLoader failed\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    fmt::print("[graphics] OpenGL version: {}\n", glGetString(GL_VERSION));
    fmt::print("[graphics] OpenGL renderer: {}\n", glGetString(GL_RENDERER));

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}