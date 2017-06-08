#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Particle.h"
#include "Wall.h"

#include <vector>
#include <map>

enum Status{
    START,
    PAUSE
};

constexpr const char* vertex_shader_path = "../shader/pbf_vertex_shader.vert";
constexpr const char* fragment_shader_path = "../shader/pbf_fragment_shader.frag";

class ParticleSystem
{
    GLFWwindow *glfw_window;

    Status system_status;

    float kernal_neighbor_distance;
    float system_max_x, system_max_z;

    float cell_size;
    long cell_max_x, cell_max_z, xz_layer_cell_size;

    std::map<long, std::vector<Particle>> system_hash_grid;

    // particle.
    std::vector<Particle> init_particles;
    std::vector<Particle> system_particles;
    std::vector<glm::vec3> particles_posi;

    // wall.
    std::vector<Wall>     system_walls;
    std::vector<glm::vec3> wall_vertex_posi;


    GLuint particles_VAO, particles_VBO;
    GLuint wall_VBO, wall_VAO;

    Shader my_shader;

    Camera my_camera;
public:

    explicit ParticleSystem(GLFWwindow *window, float system_x, float system_z, float kernal_dis, float cell_size);

    void add_rect_water(int num, glm::vec3 bottom_center, float width, float height);
    void add_wall(const Wall &wall);

    void update(float delta_t);
    void draw_scene();

private:
    
    void init_draw_wall();
    void init_draw_particle();
    void cal_hashtable_for_particles();
    long cell_hash(long x, long y, long z);
    
public:
    void process_input(float deltaTime)
	{
		if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(glfw_window, true);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(FORWARD, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(BACKWARD, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(LEFT, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(RIGHT, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(UP, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
            my_camera.ProcessKeyBoard(DOWN, deltaTime);
		}

		if (glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS)
		{
            system_status = PAUSE;
        }
        
        if (glfwGetKey(glfw_window, GLFW_KEY_O) == GLFW_PRESS)
        {
            system_status = START;
        }

        if (glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS)
        {
            reset();
        }
	}
    
    void mouse_movement(double xpos, double ypos)
	{
		static float lastX, lastY;
		static bool first_enter = true;
		if (first_enter)
		{
			first_enter = false;
			lastX = xpos;
			lastY = ypos;
		}

        my_camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);

		//        std::cout << "xpos: " << xpos << std::endl;
		//        std::cout << "ypos: " << ypos << std::endl;

		lastX = xpos;
		lastY = ypos;
	}

    void mouse_scroll(double xoffset, double yoffset)
	{
        (void)xoffset;
		my_camera.ProcessMouseScroll(yoffset);
	}

	void start()
	{
		system_status = START;
	}

	void pause()
	{
		system_status = PAUSE;
	}

	void reset()
	{
		system_status = PAUSE;

        std::cout << "init particles: " << init_particles.size() << std::endl;

		system_particles = init_particles;
		int sz = init_particles.size();
		for(int i = 0; i < sz; i++)
		{
			particles_posi[i] = init_particles[i].position;
		}
	}
};

#endif
