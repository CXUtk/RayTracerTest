#pragma once
#include "Structure/Material.h"
class MetalMat : public Material {
public:
    MetalMat();
    MetalMat(const glm::vec3& color, float fuzz) :Material(color), _fuzz(fuzz) {}
    virtual bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;
private:
    float _fuzz;
};
