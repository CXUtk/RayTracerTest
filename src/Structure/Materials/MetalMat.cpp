#include "MetalMat.h"
#include "Structure/Utils.h"
MetalMat::MetalMat() :_fuzz(0) {
}
bool MetalMat::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    auto N = info.getNormal();
    auto reflect = glm::normalize(glm::reflect(ray.getDir(), N) + randSphereVec() * _fuzz);
    attenuation = getDiffuseColor();
    new_ray = Ray(info.getHitPos() + reflect * 0.0001f, reflect);
    return glm::dot(reflect, N) > 0;
}
