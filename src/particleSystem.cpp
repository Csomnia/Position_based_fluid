#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float dis)
    : system_status(PAUSE),
      kernal_neighbor_distance(dis),
      my_shader(vertex_shader_path, fragment_shader_path)
{

}

inline void ParticleSystem::init_draw_wall()
{
    std::cout << "wall vertex size: " << wall_vertex_posi.size() << std::endl;
    glGenVertexArrays(1, &wall_VAO);
    glGenBuffers(1, &wall_VBO);

    glBindVertexArray(wall_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, wall_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * wall_vertex_posi.size(),
                 &wall_vertex_posi[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

inline void ParticleSystem::init_draw_particle()
{
    glGenVertexArrays(1, &particles_VAO);
    glGenBuffers(1, &particles_VBO);

    glBindVertexArray(particles_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, particles_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_posi.size(),
                 NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

void ParticleSystem::draw_scene()
{
    // set shader.
    my_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(app_camera.Zoom), 800.f / 600,
                                            0.1f, 100.0f);

    my_shader.setMat4("model", glm::mat4());
    my_shader.setMat4("view", app_camera.GetViewMatrix());
    my_shader.setMat4("projection", projection);

//    std::cout << "Front : " << app_camera.Front.x << " " << app_camera.Front.y << " " << app_camera.Front.z << std::endl;

    // set draw wire frame.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // draw wall.
    glBindVertexArray(wall_VAO);
    glDrawArrays(GL_TRIANGLES, 0, wall_vertex_posi.size());
    glBindVertexArray(0);

    // draw particles.
    glBindVertexArray(particles_VAO);
    glPointSize(5.f);
    glDrawArrays(GL_POINTS, 0, particles_posi.size());

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_posi.size(),
                 NULL, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particles_posi.size(),
                 &particles_posi[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ParticleSystem::update(float delta_t)
{
    if (system_status == PAUSE)
    {
        return;
    }

    int sz = system_particles.size();
    for(int i = 0; i < sz; i++)
    {
        system_particles[i].last_position = system_particles[i].position;

        system_particles[i].volicity += delta_t * system_particles[i].acceleration;
        system_particles[i].position += delta_t * system_particles[i].volicity;
        particles_posi[i] = system_particles[i].position;
    }
    std::cout << "update done" << std::endl;
}

void ParticleSystem::add_rect_water(int num, glm::vec3 bottom_center, float x_width, float z_height)
{
    int width_side_particle_num = floor(x_width / kernal_neighbor_distance) + 1;
    int height_side_particle_num = floor(z_height / kernal_neighbor_distance) + 1;

    float particle_y = bottom_center.y;

    // generate particles.
    int cnt = 0;
    while (cnt < num)
    {
        float particle_x = bottom_center.x - (x_width / 2);

        for(int i = 0; i < width_side_particle_num; i++)
        {
            float particle_z = bottom_center.z - (z_height / 2);
            for(int j = 0; j < height_side_particle_num; j++)
            {
                system_particles.push_back(Particle(glm::vec3(particle_x, particle_y, particle_z)));
                particles_posi.push_back(glm::vec3(particle_x, particle_y, particle_z));
                std::cout << "particle: (" << particle_x << ", "
                          << particle_y << ", "
                          << particle_z << ")"<< std::endl;
                cnt++;
                particle_z += kernal_neighbor_distance;
            }

            particle_x += kernal_neighbor_distance;
        }
        if (cnt >= num)
           break;

        particle_y += kernal_neighbor_distance;
    }
    std::cout << "particles cnt: " << system_particles.size() << std::endl;

    init_draw_particle();
}

void ParticleSystem::add_wall(const Wall &wall)
{
    // assert wall vertex size equal 4.
    system_walls.push_back(wall);

    // add wall vertex.
    for (const glm::vec3 &w_vertex: wall.get_vertexes())
    {
        wall_vertex_posi.push_back(w_vertex);
    }

    init_draw_wall();
}

