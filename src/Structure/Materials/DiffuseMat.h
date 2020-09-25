#pragma once
#include "Structure/Material.h"
class DiffuseMat : public Material {
public:
    DiffuseMat();
    DiffuseMat(const glm::vec3& color);
    virtual bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;
};
