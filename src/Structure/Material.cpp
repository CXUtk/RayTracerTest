#include "Material.h"

Material::Material() : _diffuseColor(glm::vec3(1)) {
}

Material::~Material() {
}

glm::vec3 Material::getColor(const Ray& ray, const IntersectionInfo& info) {
    return glm::vec3(0);
}

bool Material::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    attenuation = _diffuseColor;
    return false;
}
