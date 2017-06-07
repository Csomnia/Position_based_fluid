#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "AppCamera.h"
#include "InputHandler.hpp"
#include "ParticleSystem.h"

std::vector<glm::vec3> wall_1 {
    glm::vec3(0.9, 0.1, 0.9),
    glm::vec3(0.9, 0.1, -0.9),
    glm::vec3(-0.9, 0.1, -0.9),
    glm::vec3(-0.9, 0.1, 0.9),
    glm::vec3(0.9, 0.1, 0.9),
    glm::vec3(-0.9, 0.1, -0.9)
};

Camera app_camera{};
float last_frame;
application app("Hello pbf!", 800, 600);

int main()
{
    if (!app.init())
    {
        return -1;
    }
    auto app_window = app.get_window();

    // set input handler.
    glfwSetInputMode(app_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(app_window, mouse_movement_callback);
    glfwSetScrollCallback(app_window, mouse_scroll_callback);

    ParticleSystem particleSystem(0.05f);

    //
    particleSystem.add_wall(Wall(wall_1));
    particleSystem.add_rect_water(1000, glm::vec3(0.0f, 0.5f, 0.0f),
                                  0.6, 0.6);

    particleSystem.start();
    // main loop
    while (!glfwWindowShouldClose(app_window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float now_time = glfwGetTime();
        float delta_time = now_time - last_frame;
        last_frame = now_time;

        process_input(app_window, delta_time);


        particleSystem.draw_scene();

        particleSystem.update(delta_time);



        glfwPollEvents();
        glfwSwapBuffers(app_window);
    }

    app.close();

    return 0;
}


