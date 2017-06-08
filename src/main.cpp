#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#include "ParticleSystem.h"

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos);

std::vector<glm::vec3> wall_1 {
    glm::vec3(0.0f, 0.1f, 0.0f),
    glm::vec3(0.64f, 0.1f, 0.0f),
    glm::vec3(0.64f, 0.1f, 0.64f),
    glm::vec3(0.0f, 0.1f, 0.64f),
};

Camera app_camera{};
float last_frame;
application app("Hello pbf!", 1024, 768);
std::shared_ptr<ParticleSystem> simulation;

int main()
{
    if (!app.init())
    {
        return -1;
    }

    auto app_window = app.get_window();

    // set input handler.
//    glfwSetInputMode(app_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(app_window, mouse_movement_callback);
    glfwSetScrollCallback(app_window, mouse_scroll_callback);

    // initial a particleSystem  x , y, kernal_h, cell_size
    simulation = std::make_shared<ParticleSystem>(app_window, 0.64f, 0.64f, 0.1f, 0.1f);

    //
    simulation->add_wall(Wall(wall_1));
    simulation->add_rect_water(100, glm::vec3(0.32f, 0.5f, 0.32f),
                                  0.3, 0.3);

    // main loop
    while (!glfwWindowShouldClose(app_window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float now_time = glfwGetTime();
        float delta_time = now_time - last_frame;
        last_frame = now_time;

        simulation->process_input(delta_time);

        simulation->draw_scene();

        simulation->update(delta_time);


        glfwPollEvents();
        glfwSwapBuffers(app_window);
    }

    app.close();

    return 0;
}

void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
    (void)window;
    simulation->mouse_movement(xpos, ypos);
}

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    (void)window;
    simulation->mouse_scroll(xoffset, yoffset);
}

