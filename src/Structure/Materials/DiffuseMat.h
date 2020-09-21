#pragma once
#include "Structure/Material.h"
class DiffuseMat : public Material {
public:
    DiffuseMat();

    virtual bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;
};
