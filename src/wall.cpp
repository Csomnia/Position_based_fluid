#include "Wall.h"

void Wall::collision_response(Particle &p) const
{
    float diff = glm::dot(glm::normalize(p.position - plane_center), plane_normal);

//    std::cout << "diff: " << diff << std::endl;

    // not collision.
    if (diff > 0)
        return;

    // dectected collision
    glm::vec3 line_dir = glm::normalize(p.position - p.last_position);

    // line speed.
    float p_speed_length = glm::length(p.volicity);

    // find line-plane intersect point.
    float d = glm::dot((plane_center - p.last_position), plane_normal) / glm::dot(line_dir, plane_normal);

    glm::vec3 intersect_point = p.last_position + d * line_dir;

    glm::vec3 in_ray = glm::normalize(p.position - p.last_position);

    glm::vec3 reflect_dir = glm::reflect(in_ray, plane_normal);


//    std::cout << "in ray: ("
//              << in_ray.x << ", "
//              << in_ray.y << ", "
//              << in_ray.z << ") " << std::endl;

//    std::cout << "reflect dir: ("
//              << reflect_dir.x << ", "
//              << reflect_dir.y << ", "
//              << reflect_dir.z << ") " << std::endl;

//    std::cout << "line speed: "
//              << p_speed_length << std::endl;

    p.position = intersect_point;

    p.volicity = p_speed_length * reflect_dir * 0.5f;
}
