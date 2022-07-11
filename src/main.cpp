#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

int main(int args, const char** argv)
{
    SPDLOG_INFO("start program");

    SPDLOG_INFO("init glfw");
    if (!glfwInit())
    {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to init glfw: {}", description);
        return -1;
    }

    SPDLOG_INFO("create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    SPDLOG_INFO("start main loop");
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
