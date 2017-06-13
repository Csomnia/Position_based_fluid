#ifndef _COLLSION_HANDLE_H
#define _COLLSION_HANDLE_H

#include <glm/vec3.hpp>
#include "Particle.h"

class CollsionHandle
{
    glm::vec3 normal;
    glm::vec3 point_in_plane;
public:
    CollsionHandle(const glm::vec3 &norm, const glm::vec3 &point)
        : normal(norm),
          point_in_plane(point)
    {}

    void collison_response(Particle &p)
    {
    }
};


#endif
