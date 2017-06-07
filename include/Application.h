#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <memory>

#include "AppCamera.h"

class application
{
    const char *vertex_shader_path = "../shader/pbf_vertex_shader.vert";
    const char *fragment_shader_path = "../shader/pbf_fragment_shader.frag";
    const std::string window_title;

    int window_width, window_height;
    GLFWwindow *window;

public:
    application(const std::string &title, int width, int height)
        : window_title(title), window_width(width), window_height(height)
    {}

    GLFWwindow *get_window()
    {
        return window;
    }

    bool init();
    void event_loop();
    void close();

};

#endif
