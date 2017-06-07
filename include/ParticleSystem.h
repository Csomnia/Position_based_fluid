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
#include "AppCamera.h"

#include <vector>

enum Status{
    START,
    PAUSE
};

constexpr const char* vertex_shader_path = "../shader/pbf_vertex_shader.vert";
constexpr const char* fragment_shader_path = "../shader/pbf_fragment_shader.frag";

class ParticleSystem
{

    Status system_status;
    float kernal_neighbor_distance;

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

public:
    explicit ParticleSystem(float dis);

    void add_rect_water(int num, glm::vec3 bottom_center, float width, float height);
    void add_wall(const Wall &wall);

    void update(float delta_t);
    void draw_scene();

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

        system_particles = init_particles;
        int sz = init_particles.size();
        for(int i = 0; i < sz; i++)
        {
            particles_posi[i] = init_particles[i].position;
        }
    }


private:
    void init_draw_wall();
    void init_draw_particle();
};

#endif
