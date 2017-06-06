#include "application.h"

bool application::init()
{
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);

    if (!window)
    {
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    return true;
}

void application::event_loop()
{
    while (!glfwWindowShouldClose(window))
    {
        process_input();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void application::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

inline void application::process_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void application::close()
{
    glfwTerminate();
}
