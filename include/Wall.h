#ifndef _WALL_H
#define _WALL_H

#include "glm/glm.hpp"
#include "Particle.h"
#include <vector>
#include <iostream>

class Wall
{
    std::vector<glm::vec3> wall_vertexes;

    glm::vec3 plane_center;
    glm::vec3 plane_normal;

public:
    explicit Wall(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &p4)
    {
        wall_vertexes.push_back(p1);
        wall_vertexes.push_back(p2);
        wall_vertexes.push_back(p3);
        wall_vertexes.push_back(p4);

        plane_normal = glm::normalize(glm::cross(p4 - p1, p2 - p1));
        plane_center = (p3 + p1) / 2.0f;
    }
    explicit Wall(const std::vector<glm::vec3> &vertexes)
        : wall_vertexes(vertexes)
    {
        plane_normal = glm::normalize(glm::cross(vertexes[1] - vertexes[0],
                                                 vertexes[3] - vertexes[0]));
        plane_center = (vertexes[2] + vertexes[0]) / 2.0f;

        std::cout << "plane_center: ("
                  << plane_center.x << ", "
                  << plane_center.y << ", "
                  << plane_center.z << ") " << std::endl;

        std::cout << "plane_normal: ("
                  << plane_normal.x << ", "
                  << plane_normal.y << ", "
                  << plane_normal.z << ") " << std::endl;
    }

    const std::vector<glm::vec3> &get_vertexes() const
    {
        return wall_vertexes;
    }

    void collision_response_projection(Particle &p) const
    {
        float diff = glm::dot(glm::normalize(p.position - plane_center), plane_normal);

        // not collision.
        if (diff > 0)
            return;

        p.position = p.position - (glm::dot((p.position - plane_center), plane_normal) * plane_normal);
    }

    void collision_for_delta_p_intersection(Particle &p) const
    {
        // from https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection

        float diff = glm::dot(glm::normalize((p.position + p.delta_p) - plane_center), plane_normal);

        if (diff > 0)
            return;

        //std::cout << "collision happend." << std::endl;

        // glm::vec3 line_dir = glm::normalize(p.position - p.last_position);
        float d = glm::dot((plane_center - p.position), plane_normal) / glm::dot(p.delta_p, plane_normal);

        p.delta_p = (p.position + d * p.delta_p) - p.position;

    }

    void collision_for_delta_p_projection(Particle &p) const
    {
        float diff = glm::dot(glm::normalize((p.position + p.delta_p) - plane_center), plane_normal);

        if (diff > 0)
        {
            return;
        }
        glm::vec3 new_point = p.position + p.delta_p;

        p.delta_p = (new_point - (glm::dot(new_point - plane_center, plane_normal) * plane_normal)) - p.position;
    }

};
#endif
