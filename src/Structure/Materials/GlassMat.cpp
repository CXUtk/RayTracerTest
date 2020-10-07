#include "GlassMat.h"
#include "Structure/Utils.h"
GlassMat::GlassMat() {
    _reflectiveIndex = 1.5f;
}
GlassMat::GlassMat(float reflectiveidx) :_reflectiveIndex(reflectiveidx) {
}
glm::vec3 GlassMat::getColor(const Ray& ray, const IntersectionInfo& info) {
    return glm::vec3(0);
}


float schlick(float cost, float eta) {
    auto r0 = (1 - eta) / (1 + eta);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cost, 5.0f);
}

glm::vec3 refract(const glm::vec3& r, const glm::vec3& N, float eta) {
    auto cost = glm::dot(-N, r);
    auto A = -N * cost;
    auto para = A * eta;
    auto perp2 = (r - A) * eta;
    auto t = glm::dot(perp2, perp2);
    if (t > 1)
        return glm::reflect(r, N);
    auto sint = std::sqrt(1 - t);
    float reflect_prob = schlick(cost, eta);
    if (randFloat() < reflect_prob)
        return glm::reflect(r, N);
    return perp2 - sint * N;

}
bool GlassMat::scatter(const Ray& ray, const IntersectionInfo& info, glm::vec3& attenuation, Ray& new_ray) {
    attenuation = glm::vec3(1);
    float etai_over_etat = info.isFrontFace() ? (1.0 / _reflectiveIndex) : _reflectiveIndex;
    // refract(ray.getDir(), info.getNormal(), etai_over_etat);//
    glm::vec3 dir = refract(ray.getDir(), info.getNormal(), etai_over_etat);
    new_ray = Ray(info.getHitPos() + dir * 0.0001f, dir);
    return true;
}
