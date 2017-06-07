#ifndef _WALL_H
#define _WALL_H

#include "glm/vec3.hpp"
#include "Particle.h"
#include <vector>

class Wall
{
    std::vector<glm::vec3> wall_vertexes;
public:
    Wall(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
        : wall_vertexes{p1, p2, p3, p4}
    {}

    Wall(std::vector<glm::vec3> vertexes)
        : wall_vertexes(vertexes)
    {}

    const std::vector<glm::vec3> &get_vertexes() const
    {
        return wall_vertexes;
    }

    void collision_response(Particle &p);
};
#endif
