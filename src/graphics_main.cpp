#include "cmd.hpp"
#include "game_state.hpp"
#include "imgui/console.hpp"
#include "imgui/imgui.hpp"
#include "tsqueue.hpp"

#include <entt/entt.hpp>
#include <fmt/printf.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void glfw_error_callback(int code, const char* msg)
{
    fmt::print("[GLFW] error {:d}: {:s}\n", code, msg);
}

void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: fmt::print("[OpenGL] error: {:s}", message); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: fmt::print("[OpenGL] deprecation warning: {:s}", message); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: fmt::print("[OpenGL] undefined behavior warning: {:s}", message); break;
    case GL_DEBUG_TYPE_PORTABILITY: fmt::print("[OpenGL] portability warning: {:s}", message); break;
    case GL_DEBUG_TYPE_PERFORMANCE: fmt::print("[OpenGL] performance warning: {:s}", message); break;
    default: fmt::print("[OpenGL] debug: {:s}", message);
    }
}

static bool show_console = true;

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_T && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL))
    {
        show_console = !show_console;
    }
}

void graphics_main(entt::registry& registry)
{
    auto& state = registry.ctx().at<GameState>();

    if (!glfwInit())
    {
        fmt::print("glfwInit failed\n");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    auto monitor = glfwGetPrimaryMonitor();
    auto mode = glfwGetVideoMode(monitor);
    auto window = glfwCreateWindow(mode->width, mode->height, "Pineapple Republic", monitor, nullptr);
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

    glDebugMessageCallback(gl_debug_callback, nullptr);

    glfwSetKeyCallback(window, glfw_key_callback);

    fmt::print("[graphics] OpenGL version: {}\n", glGetString(GL_VERSION));
    fmt::print("[graphics] OpenGL renderer: {}\n", glGetString(GL_RENDERER));

    imgui::init(window);
    imgui::Console console;
    console.set_cmd_callback([&](const std::string& cmd)
    {
        auto& cmd_queue = registry.ctx().at<TsQueue<std::string>>();
        cmd_queue.push(cmd);
    });

    while (state.this_thread().running && !glfwWindowShouldClose(window))
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);
        // TODO: rendering

        auto& log_queue = registry.ctx().at<TsQueue<cmd::CommandResult>>();
        while (!log_queue.is_empty())
        {
            console.push_log_entry(log_queue.pop().msg);
        }

        imgui::begin();
        if (show_console) { console.draw(&show_console); }
        imgui::draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imgui::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}