#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <glm/vec3.hpp>

class Particle
{
public:
    glm::vec3 position;
    glm::vec3 last_position;
    glm::vec3 volicity;
    glm::vec3 acceleration;
    glm::vec3 external_force;
    float mass;

    Particle(const glm::vec3 &posi)
        : position(posi),
          last_position(0),
          volicity(0),
          acceleration(0.0f, -0.98f, 0.0f),
          external_force(0.0f, -9.8f, 0.0f),
          mass(0)
    {}
};

#endif
