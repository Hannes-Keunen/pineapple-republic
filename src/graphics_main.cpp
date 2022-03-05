#include "cmd.hpp"
#include "event.hpp"
#include "game_state.hpp"
#include "gfx/batch.hpp"
#include "gfx/camera.hpp"
#include "gfx/gl/texture.hpp"
#include "log.hpp"
#include "imgui/console.hpp"
#include "imgui/imgui.hpp"
#include "imgui/log_window.hpp"
#include "imgui/renderer_stats.hpp"
#include "res/cache.hpp"
#include "tsqueue.hpp"

#include <entt/entt.hpp>
#include <fmt/printf.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>

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

void draw_frame(entt::registry& registry, gfx::Batch& batch);

void graphics_main(entt::registry& registry)
{
    auto& state = registry.ctx().at<GameState>();

    if (!glfwInit())
    {
        state.log_e("glfwInit failed");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // auto monitor = glfwGetPrimaryMonitor();
    // auto mode = glfwGetVideoMode(monitor);
    // auto window = glfwCreateWindow(mode->width, mode->height, "Pineapple Republic", monitor, nullptr);
    auto window = glfwCreateWindow(1920, 1080, "Pineapple Republic", nullptr, nullptr);
    if (window == nullptr)
    {
        state.log_e("failed to create a window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0)
    {
        state.log_e("gladLoadGLLoader failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    glDebugMessageCallback(gl_debug_callback, nullptr);

    glfwSetKeyCallback(window, glfw_key_callback);

    state.log_i("OpenGL version: {}", glGetString(GL_VERSION));
    state.log_i("OpenGL renderer: {}", glGetString(GL_RENDERER));

    auto batch = gfx::Batch::create(1024).value();
    auto& cam = state.emplace<gfx::Camera>();
    cam.ortho(0, 16, 9, 0);

    int x, y, c;
    auto tex_data = stbi_load("../res/textures/farm_tile.bmp", &x, &y, &c, 4);
    auto& textures = state.emplace<res::Cache<gfx::gl::Texture>>();
    textures.emplace("../res/textures/farm_tile.bmp", gfx::gl::Texture::create(x, y, gfx::gl::TextureFormat::Rgba8U, tex_data).value());
    stbi_image_free(tex_data);
    tex_data = stbi_load("../res/textures/pineapple_0.bmp", &x, &y, &c, 4);
    textures.emplace("../res/textures/pineapple_0.bmp", gfx::gl::Texture::create(x, y, gfx::gl::TextureFormat::Rgba8U, tex_data).value());

    imgui::init(window);
    imgui::Console console;
    console.set_cmd_callback([&](const std::string& cmd)
    {
        auto& cmd_queue = registry.ctx().at<TsQueue<std::string>>();
        cmd_queue.push(cmd);
    });
    imgui::LogWindow log_window(state);
    imgui::RendererStatsWindow stats_window;

    while (state.this_thread().running && !glfwWindowShouldClose(window))
    {
        // Events
        auto& logger = state.get<EventBus<logging::Entry>>();
        logger.drain();

        auto& log_queue = registry.ctx().at<TsQueue<cmd::CommandResult>>();
        while (!log_queue.is_empty())
        {
            console.push_log_entry(log_queue.pop().msg);
        }

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_frame(registry, batch);

        imgui::begin();
        if (show_console) { console.draw(&show_console); }
        log_window.draw();
        stats_window.draw(batch);
        imgui::draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imgui::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void draw_frame(entt::registry& registry, gfx::Batch& batch)
{
    auto& state = registry.ctx().at<GameState>();
    auto& cam = state.get<gfx::Camera>();
    auto& textures = state.get<res::Cache<gfx::gl::Texture>>();
    auto tex1 = textures.get("../res/textures/farm_tile.bmp");
    auto tex2 = textures.get("../res/textures/pineapple_0.bmp");

    auto frame = state.begin_frame();
    batch.begin(cam.vp_matrix());
    for (int x = 0; x < 16; x +=  1) {
        for (int y = 0; y < 9; y +=  1) {
            batch.submit(x, y, 1, 1, ((x + y) % 2 == 0) ? tex1 : tex2);
        }
    }
    batch.end();
}
