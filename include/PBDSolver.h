#ifndef _PBDSOLVER_H
#define _PBDSOLVER_H


#include <glm/glm.hpp>
#include <vector>

#include "Particle.h"

constexpr const float PI = 3.1415926535897932384626433832795028;

class PBDSolver
{
public:
    PBDSolver() = default;

    static glm::vec3 calc_gradient_spiky_kernel(const glm::vec3 &r_vec, float h)
    {
        // kernel form [Muller et al. 2003]
        const float spiky_kernel_const = - 45.0f / (PI * glm::pow(h, 6));
        float r_dis = glm::length(r_vec);

        if (r_dis > h)
        {
//            std::cout << "out of radius" << std::endl;
            return glm::vec3(0);
        }

        if (r_dis == 0)
        {
//            std::cout << "r_dis equal 0" << std::endl;
            return glm::vec3(0);
        }

        float coff = (spiky_kernel_const * glm::pow(h - r_dis, 2)) / r_dis;

        return glm::vec3(coff * r_vec.x,
                         coff * r_vec.y,
                         coff * r_vec.z);
    }

    static void calc_lambda(Particle &center_p, const float h, const float density_const)
    {
        // formula from [PBF Macklin .2013]
        float square_gradient_sum = 0.0f;

        // calculation for case1 constrain. [PBF Macklin . 2013] formula (8)
        glm::vec3 case1_gradient_constrain(0.0);

        // p is a pointer to center particle's neightbor
        for(const auto &p : center_p.neighbors)
        {
            glm::vec3 r_vec = center_p.position - p->position;

            glm::vec3 tmp_gradient  = calc_gradient_spiky_kernel(r_vec, h);

            case1_gradient_constrain += tmp_gradient;
        }

        case1_gradient_constrain /= density_const;

        square_gradient_sum += get_square_sum(case1_gradient_constrain);

        if (square_gradient_sum != square_gradient_sum)
        {
            std::cout << "here" << std::endl;
        }

        // calulation for case2 constrain. [PBF Macklin .2013] formula (8)
        for(const auto &p : center_p.neighbors)
        {
            glm::vec3 r_vec = center_p.position - p->position;

            // note: a minus sign here.
            glm::vec3 case2_gradient_constrain = - calc_gradient_spiky_kernel(r_vec, h);

            case2_gradient_constrain /= density_const;
            square_gradient_sum += get_square_sum(case2_gradient_constrain);
        }
//        std::cout << "density constrain: " << calc_density_constrain(center_p, h, density_const) << std::endl;
//        std::cout << "square gradietn sum: " << square_gradient_sum << std::endl;

        float constrain = calc_density_constrain(center_p, h, density_const);

        // std::cout << "square gradient : " <<  square_gradient_sum << std::endl;

        center_p.p_lambda = - constrain / (square_gradient_sum + 8000);

        if (center_p.p_lambda != center_p.p_lambda)
        {
            std::cout << "here" << std::endl;
        }

    }

    static void calc_delta_p(Particle &center_p, float h, float density_const)
    {
        if (center_p.p_lambda != center_p.p_lambda)
        {
            std::cout << "here" << std::endl;
        }
        glm::vec3 lambda_gradient_sum(0);

        const float tensile_denominator = (315.0f / (64.0f * PI * glm::pow(h, 9))) * glm::pow((h * h), 3);

        for(const auto &p: center_p.neighbors)
        {
            float scorr = - 0.001 * glm::pow(calc_Wpoly6(center_p.position - p->position, h) /
                                                                 tensile_denominator
                                                                 , 4);

            lambda_gradient_sum += (center_p.p_lambda + p->p_lambda + scorr) * calc_gradient_spiky_kernel(center_p.position - p->position, h);
        }
        center_p.delta_p = lambda_gradient_sum / density_const;
        if (center_p.delta_p.x != center_p.delta_p.x ||
            center_p.delta_p.y != center_p.delta_p.y ||
            center_p.delta_p.z != center_p.delta_p.z)
        {
            std::cout << "here" << std::endl;
        }
    }

private:
    static float get_square_sum(const glm::vec3 &r_vec)
    {
        return r_vec.x * r_vec.x + r_vec.y * r_vec.y + r_vec.z * r_vec.z;
    }

    static float calc_Wpoly6(const glm::vec3 &diff, float h)
    {
        const float poly6_const = 315.0f / (64.0f * PI * glm::pow(h, 9));
        const float h_square = h * h;

        return poly6_const * glm::pow(h_square - glm::dot(diff, diff), 3);
    }

    static float calc_density_constrain(const Particle &center_p, float h, float density_const)
    {
        const float poly6_const = 315.0f / (64.0f * PI * glm::pow(h, 9));
        const float h_square = h * h;

        float density_estimate = 0.0f;

        for(const auto &p :  center_p.neighbors)
        {
            float r_square = get_square_sum(center_p.position - p->position);

            if (h_square > r_square)
            {
                density_estimate += poly6_const * glm::pow(h_square - r_square, 3);
            }
        }

        return (density_estimate / density_const) - 1.0f;
    }
};


#endif
