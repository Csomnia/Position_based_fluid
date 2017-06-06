#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class application
{
    const std::string window_title;
    int window_width, window_height;
    GLFWwindow *window;

public:
    application(const std::string &title, int width, int height)
        : window_title(title), window_width(width), window_height(height)
    {}

    bool init();
    void event_loop();
    void close();

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void process_input();
};

#endif
