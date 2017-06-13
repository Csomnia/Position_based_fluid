#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

#include "ParticleSystem.h"

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos);

#define BOX_SIZE 0.32f
#define TOP_SIZE 0.64f

std::vector<glm::vec3> wall_1 {         // bottom
    glm::vec3(BOX_SIZE, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, BOX_SIZE),
    glm::vec3(BOX_SIZE, 0.0f, BOX_SIZE),
};

std::vector<glm::vec3> wall_2 {        // front
    glm::vec3(0.0f, BOX_SIZE, BOX_SIZE),
    glm::vec3(BOX_SIZE, BOX_SIZE, BOX_SIZE),
    glm::vec3(BOX_SIZE, 0.0f, BOX_SIZE),
    glm::vec3(0.0f, 0.0f, BOX_SIZE),
};

std::vector<glm::vec3> wall_3 {         // left
    glm::vec3(0.0f, BOX_SIZE, 0.0f),
    glm::vec3(0.0f, BOX_SIZE, BOX_SIZE),
    glm::vec3(0.0f, 0.0f, BOX_SIZE),
    glm::vec3(0.0f, 0.0f, 0.0f),
};
std::vector<glm::vec3> wall_4 {         // back
    glm::vec3(BOX_SIZE, BOX_SIZE, 0.0f),
    glm::vec3(0.0f, BOX_SIZE, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(BOX_SIZE, 0.0f, 0.0f),
};
std::vector<glm::vec3> wall_5 {         // right
    glm::vec3(BOX_SIZE, BOX_SIZE, BOX_SIZE),
    glm::vec3(BOX_SIZE, BOX_SIZE, 0.0f),
    glm::vec3(BOX_SIZE, 0.0f, 0.0f),
    glm::vec3(BOX_SIZE, 0.0f, BOX_SIZE),
};
std::vector<glm::vec3> wall_6 {         // top
    glm::vec3(TOP_SIZE, TOP_SIZE, TOP_SIZE),
    glm::vec3(0.0f, TOP_SIZE, TOP_SIZE),
    glm::vec3(0.0f, TOP_SIZE, 0.0f),
    glm::vec3(TOP_SIZE, TOP_SIZE, 0.0f),
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
    simulation = std::make_shared<ParticleSystem>(app_window,
                                                  BOX_SIZE,  // system x
                                                  BOX_SIZE,  // system z
                                                  0.05f,   // kerynal_h
                                                  120000.f,   // density
                                                  0.05f);  // grid size

    // add walls and water.
    simulation->add_wall(Wall(wall_1), true);
    simulation->add_wall(Wall(wall_2), false);
    simulation->add_wall(Wall(wall_3), true);
    simulation->add_wall(Wall(wall_4), true);
    simulation->add_wall(Wall(wall_5), true);
    simulation->add_wall(Wall(wall_6), false);

    simulation->add_rect_water(1000, glm::vec3(0.16f, 0.25f, 0.16f),
                               0.19, 0.19, 10);


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

        simulation->update();


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

