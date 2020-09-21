#pragma once
#include "Structure/Material.h"
class NormalMat : public Material {
public:
    NormalMat();
    glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) override;
    bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;
};
