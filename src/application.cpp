#include "Application.h"

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

    // load OpenGL
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glEnable(GL_DEPTH_TEST);

    return true;
}

void application::close()
{
    glfwTerminate();
}
