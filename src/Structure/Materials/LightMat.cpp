#include "LightMat.h"
#include "Structure/Utils.h"
LightMat::LightMat(const glm::vec3& intensity) :_lightColor(intensity) {

}
glm::vec3 LightMat::getColor(const Ray& ray, const IntersectionInfo& info) {
    return _lightColor;
}
bool LightMat::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    attenuation = glm::vec3(0);
    return false;
}
