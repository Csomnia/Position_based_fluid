#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLFWwindow *window, float system_x, float system_z, float kernal_dis, float density, float cell_size)
    : glfw_window(window),
      system_status(PAUSE),
      kernal_neighbor_distance(kernal_dis),
      system_max_x(system_x),
      system_max_z(system_z),
      density_const(density),
      cell_size(cell_size),
      particles_VAO(0), particles_VBO(0),
      wall_VAO(0), wall_VBO(0),
      my_shader(vertex_shader_path, fragment_shader_path),
      my_camera(glm::vec3(0.0f, 0.0f, 2.0f))

{
    cell_max_x = ceil(system_x / cell_size);
    cell_max_z = ceil(system_z / cell_size);

    xz_layer_cell_size = cell_max_x * cell_max_z;

    std::cout << "cell max x: " << cell_max_x << std::endl;
    std::cout << "cell max z: " << cell_max_z << std::endl;
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
    if (first_draw)
    {
           init_draw_wall();
        init_draw_particle();
        first_draw = false;
    }

    // set shader.
    my_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(my_camera.Zoom), 1024.f / 768,
                                            0.1f, 100.0f);

    my_shader.setMat4("model", glm::mat4());
    my_shader.setMat4("view", my_camera.GetViewMatrix());
    my_shader.setMat4("projection", projection);

    //    std::cout << "Front : " << app_camera.Front.x << " " << app_camera.Front.y << " " << app_camera.Front.z << std::endl;

    // set draw wire frame.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // draw wall.
    glBindVertexArray(wall_VAO);
    glDrawArrays(GL_LINES, 0, wall_vertex_posi.size());
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


void ParticleSystem::trivial_update()
{
    float delta_t = 0.016;

    if (system_status == PAUSE)
    {
        return;
    }

    int sz = system_particles.size();

    for(int i = 0; i < sz; i++)
    {
        Particle &now_process_particle = system_particles[i];

        // save last postion.
        now_process_particle.last_position = now_process_particle.position;

        // update volicity use acceleration.
        now_process_particle.volicity += delta_t * now_process_particle.acceleration;

        // update position use volicity.
        system_particles[i].position += delta_t * system_particles[i].volicity;

        // collision dectect and response.
        for(const Wall &w: system_walls)
        {
            w.collision_response_projection(system_particles[i]);
        }
    }

    // copy particle postion for display.
    for(int i = 0; i < sz; i++)
    {
        particles_posi[i] = system_particles[i].position;
    }
}

void ParticleSystem::update()
{
    float delta_t = 0.016;

    if (system_status == PAUSE)
    {
        return;
    }

    int sz = system_particles.size();

    for(int i = 0; i < sz; i++)
    {
        Particle &now_process_particle = system_particles[i];

        // save last postion.
        now_process_particle.last_position = now_process_particle.position;

        // update volicity use acceleration.
         now_process_particle.volicity += delta_t * now_process_particle.acceleration;

        // update position use volicity.
        system_particles[i].position += delta_t * system_particles[i].volicity;

        // collision dectect and response.
        for(const Wall &w: system_walls)
        {
            w.collision_response_projection(system_particles[i]);
        }
    }

    // clear hash table
    system_hash_grid.clear();

    // pre calculate grid for speed up neighbor finding.
    cal_hashtable_for_particles();

    find_all_neighbors();

    for(int i = 0; i < 13; i++) {

        for(Particle &p: system_particles)
        {
            PBDSolver::calc_lambda(p, kernal_neighbor_distance, density_const);
        }

        // calulate delta_p for every particles and perfrom collision detection and response.
        for(Particle &p: system_particles)
        {
            PBDSolver::calc_delta_p(p, kernal_neighbor_distance, density_const);
            for(const auto &w : system_walls)
            {
                w.collision_for_delta_p_projection(p);
            }
        }

        for(Particle &p: system_particles)
        {
            p.position = p.position + p.delta_p;
        }
    }

    for(Particle &p: system_particles)
    {
        p.volicity = (p.position - p.last_position) / delta_t;
    }

    // copy particle postion for display.
    for(int i = 0; i < sz; i++)
    {
        particles_posi[i] = system_particles[i].position;

    }
}

void ParticleSystem::cal_hashtable_for_particles()
{
    for (Particle &p : system_particles)
    {
        // calulate the particle now placed in and get hash value.
        long hash_value = cell_hash(p.position.x / cell_size,
                                    p.position.y / cell_size,
                                    p.position.z / cell_size);

        // insert into table.
        if (system_hash_grid.count(hash_value) == 0)
            system_hash_grid.emplace(hash_value, std::vector<Particle*>{&p});
        else
            system_hash_grid[hash_value].push_back(&p);
    }
}

inline long ParticleSystem::cell_hash(long x, long y, long z)
{
    return (y * xz_layer_cell_size) + (z * cell_max_x) + x;
}

void ParticleSystem::find_all_neighbors()
{
    // for all particles find thire neighbors.
    for(Particle &center_p : system_particles)
    {
        // clear center particle's old neighbors.
        center_p.neighbors.clear();

        // get center parictle's positon.
        glm::vec3 &center_posi = center_p.position;
        long grid_x = center_posi.x / cell_size;
        long grid_y = center_posi.y / cell_size;
        long grid_z = center_posi.z / cell_size;

        // find neighbors in adjacent grid.
        for(const auto &arr : delta_xyz)
        {
            long neighbor_grid_x = grid_x + arr[0];
            long neighbor_grid_y = grid_y + arr[1];
            long neighbor_grid_z = grid_z + arr[2];

            if (neighbor_grid_x < 0 || neighbor_grid_x >= cell_max_x ||
                neighbor_grid_z < 0 || neighbor_grid_z >= cell_max_z ||
                neighbor_grid_y < 0)
            {
                continue;  // illeagl grid index.
            }

            auto it = system_hash_grid.find(cell_hash(neighbor_grid_x,
                                                      neighbor_grid_y,
                                                      neighbor_grid_z));
            // this grid has no particles.
            if (it == system_hash_grid.end())
            {
                continue;
            }

            // calculate distance for all paricle in grid,
            // to determind is neighbor or not.
            for(auto &p: it->second)
            {
                if (p == &center_p) continue; // not include the center particle self.
                float dis = glm::distance(center_p.position, p->position);
                if (dis <= kernal_neighbor_distance) {
                        center_p.neighbors.push_back(p); // not insert center particle.
                }
            }
        }

    }
}

void ParticleSystem::add_rect_water(int num, glm::vec3 bottom_center, float x_width, float z_height, int per_h)
{
    const float step = PARTICLE_RADIUS;

    int width_side_particle_num = floor(x_width / step) + 1;
    int height_side_particle_num = floor(z_height / step) + 1;

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
                //                std::cout << "particle: (" << particle_x << ", "
                //                          << particle_y << ", "
                //                          << particle_z << ")"<< std::endl;
                cnt++;
                particle_z += step;
            }

            particle_x += step;
        }
        if (cnt >= num)
            break;

        particle_y += step;
    }
    // copy particles to init_particles for initialzation.
    init_particles = system_particles;

    std::cout << "particles cnt: " << system_particles.size() << std::endl;

}


