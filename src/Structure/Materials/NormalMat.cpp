#include "NormalMat.h"
#include "Structure/Utils.h"
NormalMat::NormalMat() {
}
glm::vec3 NormalMat::getColor(const Ray& ray, const IntersectionInfo& info) {
    return 0.5f * (info.getNormal() + glm::vec3(1));
}
bool NormalMat::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    attenuation = glm::vec3(0);
    return false;
}
