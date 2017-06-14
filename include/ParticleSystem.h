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
#include "PBDSolver.h"

#include <vector>
#include <map>
#include <unordered_map>

enum Status{
    START,
    PAUSE
};

constexpr const char* particle_vertex_shader_path = "../shader/particle_vertex_shader.vert";
constexpr const char* particle_fragment_shader_path = "../shader/particle_fragment_shader.frag";

constexpr const char* wall_vertex_shader_path = "../shader/wall_vertex_shader.vert";
constexpr const char* wall_fragment_shader_path = "../shader/wall_fragment_shader.frag";

constexpr const int delta_xyz[27][3] = {
    // x  y  z
    {-1, -1, -1}, {0, -1, -1},  {1, -1, -1},
    {-1, -1, 0},  {0, -1, 0},   {1, -1, 0},
    {-1, -1, 1},  {0, -1, 1},   {1, -1, 1},

    {-1, 0, -1},  {0, 0, -1},   {1, 0, -1},
    {-1, 0, 0},   {0, 0, 0},    {1, 0, 0},
    {-1, 0, 1},   {0, 0, 1},    {1, 0, 1},

    {-1, 1, -1},  {0, 1, -1},   {1, 1, -1},
    {-1, 1, 0},   {0, 1, 0},    {1, 1, 0},
    {-1, 1, 1},   {0, 1, 1},    {1, 1, 1}
};

class ParticleSystem
{
    bool test_flag = false;
    bool first_draw = true;

    GLFWwindow *glfw_window;

    Status system_status;

    float kernal_neighbor_distance;
    float system_max_x, system_max_z;
    
    float density_const;

    float cell_size;
    long cell_max_x, cell_max_z, xz_layer_cell_size;

    //std::map<long, std::vector<Particle *>> system_hash_grid;

    std::unordered_map<long, std::vector<Particle *>> system_hash_grid;

    std::vector<float> lambda;

    // particle.
    std::vector<Particle> init_particles;
    std::vector<Particle> system_particles;
    std::vector<glm::vec3> particles_posi;

    // wall.
    std::vector<Wall>     system_walls;
    std::vector<glm::vec3> wall_vertex_posi;


    GLuint particles_VAO, particles_VBO;
    GLuint wall_VAO, wall_VBO;

    Shader particle_shader;
    Shader wall_shader;

    Camera my_camera;
public:

    explicit ParticleSystem(GLFWwindow *window,
                            float system_x,
                            float system_z,
                            float kernal_dis,
                            float density,
                            float cell_size);

    void add_rect_water(int num, glm::vec3 bottom_center, float width, float height, int per_h);

    void update();

    void trivial_update();

    void draw_scene();

    void add_wall(const Wall &wall, bool display)
        {
            // add wall for later use.
            system_walls.push_back(wall);

            if (!display)
                return;

            auto &wall_vertexes =wall.get_vertexes();

            // add wall vertex.
            wall_vertex_posi.push_back(wall_vertexes[0]);
            wall_vertex_posi.push_back(wall_vertexes[1]);

            wall_vertex_posi.push_back(wall_vertexes[1]);
            wall_vertex_posi.push_back(wall_vertexes[2]);

            wall_vertex_posi.push_back(wall_vertexes[2]);
            wall_vertex_posi.push_back(wall_vertexes[3]);

            wall_vertex_posi.push_back(wall_vertexes[3]);
            wall_vertex_posi.push_back(wall_vertexes[0]);

            wall_vertex_posi.push_back(wall_vertexes[0]);
            wall_vertex_posi.push_back(wall_vertexes[2]);

            wall_vertex_posi.push_back(wall_vertexes[1]);
            wall_vertex_posi.push_back(wall_vertexes[3]);

        }

private:
    
    void init_draw_wall();
    void init_draw_particle();


    void cal_hashtable_for_particles();
    long cell_hash(long x, long y, long z);

    void find_all_neighbors();
    
    void screenshot();
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
