#pragma once
#include "Structure/Material.h"
class GlassMat : public Material {
public:
    GlassMat();
    GlassMat(float reflectiveidx);
    glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) override;
    bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) override;

private:
    float _reflectiveIndex;
};
