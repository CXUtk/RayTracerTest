#pragma once
#include "Structure/Material.h"
class LightMat : public Material {
public:
    LightMat(const glm::vec3& intensity);
    glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) override;
    bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;
private:
    glm::vec3 _lightColor;
};
