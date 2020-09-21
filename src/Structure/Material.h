#pragma once
#include <glm/glm.hpp>
#include "Structure/Ray.h"
#include "IntersectionInfo.h"
class IntersectionInfo;
class Material {
public:
    Material();
    Material(const glm::vec3& diffuseColor) :_diffuseColor(diffuseColor) {}
    ~Material();

    glm::vec3 getDiffuseColor() const { return _diffuseColor; }
    void setDiffuseColor(glm::vec3 color) { _diffuseColor = color; }
    virtual glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info);
    virtual bool scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray);

private:
    glm::vec3 _diffuseColor;
};
