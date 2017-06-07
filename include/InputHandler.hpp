#ifndef _CALL_BACK_H
#define _CALL_BACK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "AppCamera.h"

inline void process_input(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        app_camera.ProcessKeyBoard(DOWN, deltaTime);
    }
}

inline void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX, lastY;
    static bool first_enter = true;
    if (first_enter)
    {
        first_enter = false;
        lastX = xpos;
        lastY = ypos;
    }

    app_camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);

//        std::cout << "xpos: " << xpos << std::endl;
//        std::cout << "ypos: " << ypos << std::endl;

    lastX = xpos;
    lastY = ypos;

}

inline void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    app_camera.ProcessMouseScroll(yoffset);
}

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#endif
