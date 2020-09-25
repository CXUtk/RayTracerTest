#include "DiffuseMat.h"
#include "Structure/Utils.h"
DiffuseMat::DiffuseMat() {
}
DiffuseMat::DiffuseMat(const glm::vec3& color) {
    setDiffuseColor(color);
}
bool DiffuseMat::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    auto N = info.getNormal();
    auto v = N + randSphereVec();
    v = glm::normalize(v);
    attenuation = getDiffuseColor();
    new_ray = Ray(info.getHitPos() + N * 0.0001f, v);
    return true;
}
