#include "game_state.hpp"
#include "gfx/batch.hpp"
#include "gfx/camera.hpp"
#include "gfx/gl/texture.hpp"
#include "gfx/sys/systems.hpp"
#include "log.hpp"
#include "imgui/console.hpp"
#include "imgui/demo.hpp"
#include "imgui/imgui.hpp"
#include "imgui/log_window.hpp"
#include "imgui/renderer_stats.hpp"
#include "imgui/window.hpp"
#include "res/cache.hpp"
#include "threading.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

void glfw_error_callback(int code, const char* msg)
{
    logger::error("GLFW error {:d}: {:s}\n", code, msg);
}

void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: logger::error("OpenGL error: {:s}", message); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: logger::warning("OpenGL deprecation warning: {:s}", message); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: logger::warning("OpenGL undefined behavior warning: {:s}", message); break;
    case GL_DEBUG_TYPE_PORTABILITY: logger::warning("OpenGL portability warning: {:s}", message); break;
    case GL_DEBUG_TYPE_PERFORMANCE: logger::warning("OpenGL performance warning: {:s}", message); break;
    default: logger::debug("OpenGL debug: {:s}", message);
    }
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL))
    {
        auto& state = *((GameState*) glfwGetWindowUserPointer(window));
        auto& manager = state.get<imgui::WindowManager>();
        manager.toggle_window(key);
    }
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GameState& state = *((GameState*) glfwGetWindowUserPointer(window));
    auto& camera = state.get<gfx::Camera>();
    camera.scale(yoffset > 0 ? 2.0f : 0.5f);
}

void graphics_main(GameState& state)
{
    if (!glfwInit())
    {
        logger::error("glfwInit failed");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    // auto monitor = glfwGetPrimaryMonitor();
    // auto mode = glfwGetVideoMode(monitor);
    // auto window = glfwCreateWindow(mode->width, mode->height, "Pineapple Republic", monitor, nullptr);
    auto window = glfwCreateWindow(1920, 1080, "Pineapple Republic", nullptr, nullptr);
    if (window == nullptr)
    {
        logger::error("failed to create a window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0)
    {
        logger::error("gladLoadGLLoader failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    glDebugMessageCallback(gl_debug_callback, nullptr);

    glfwSetWindowUserPointer(window, &state);
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);

    state.emplace<GLFWwindow*>(window);

    imgui::init(window);
    auto& window_manager = state.emplace<imgui::WindowManager>();
    window_manager.add_window<imgui::Console>(GLFW_KEY_T);
    window_manager.add_window<imgui::LogWindow>(GLFW_KEY_L, state);
    window_manager.add_window<imgui::RendererStatsWindow>(GLFW_KEY_S);
    window_manager.add_window<imgui::DemoWindow>(GLFW_KEY_D);

    auto& batch = state.emplace<gfx::Batch>(gfx::Batch::create(1024).value());
    auto& cam = state.emplace<gfx::Camera>();
    cam.ortho(0, 16, 9, 0);

    int x, y, c;
    auto tex_data = stbi_load("../res/textures/farm_tile.bmp", &x, &y, &c, 4);
    auto& textures = state.emplace<res::Cache<gfx::gl::Texture>>();
    textures.emplace("../res/textures/farm_tile.bmp", gfx::gl::Texture::create(x, y, gfx::gl::TextureFormat::Rgba8U, tex_data).value());
    stbi_image_free(tex_data);
    tex_data = stbi_load("../res/textures/pineapple_0.bmp", &x, &y, &c, 4);
    textures.emplace("../res/textures/pineapple_0.bmp", gfx::gl::Texture::create(x, y, gfx::gl::TextureFormat::Rgba8U, tex_data).value());

    logger::info("OpenGL version: {}", glGetString(GL_VERSION));
    logger::info("OpenGL renderer: {}", glGetString(GL_RENDERER));

    logger::trace("begin rendering loop");
    auto& tc = state.get<ThreadConfig>();
    while (tc.this_thread().running && !glfwWindowShouldClose(window))
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        gfx::sys::cam_control(state);
        gfx::sys::begin_draw(state);
        gfx::sys::draw_map(state);
        gfx::sys::end_draw(state);

        logger::drain();

        imgui::begin();
        imgui::draw_windows(state);
        imgui::draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imgui::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}
