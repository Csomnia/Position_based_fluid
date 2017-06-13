#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Constants.h"

#include <glm/vec3.hpp>
#include <vector>

class Particle
{
public:
    glm::vec3 position;
    glm::vec3 last_position;
    glm::vec3 volicity;
    glm::vec3 acceleration;
    glm::vec3 external_force;

    std::vector<Particle *> neighbors;

    float mass;
    float radius;
    float p_lambda;
    glm::vec3 delta_p;

    Particle(const glm::vec3 &posi)
        : position(posi),
          last_position(0),
          volicity(0),
//          acceleration(0),
          acceleration(0.f, -9.8f, 0.0f),
          external_force(0.0f, -9.8f, 0.0f),
          mass(0),
          radius(PARTICLE_RADIUS),
          p_lambda(0),
          delta_p(0)
    {}
};

#endif
