#include "Sphere.h"

Sphere::Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material) :_center(center), _radius(radius) {
    _material = material;
}

Sphere::~Sphere() {
}

BoundingBox Sphere::getBoundingBox() const {
    return BoundingBox(_center - glm::vec3(abs(_radius)), _center + glm::vec3(abs(_radius)));
}

bool Sphere::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    glm::vec3 P = ray.getStart() - _center;
    glm::vec3 d = ray.getDir();
    float a = glm::dot(d, d);
    float b = 2 * glm::dot(d, P);
    float c = glm::dot(P, P) - _radius * _radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    discrim = sqrt(discrim);
    float t1 = (-b + discrim) / (2 * a);
    float t2 = (-b - discrim) / (2 * a);
    if (t1 > t2) std::swap(t1, t2);
    float t = t1;
    if (t1 < 0) t = t2;
    if (t < 0) return false;
    info.quickSetInfo(ray, t, this);
    return true;
}

glm::vec3 Sphere::getDiffuseColor(const glm::vec3& pos) const {
    return _material->getDiffuseColor();
}

glm::vec3 Sphere::getSampleColor(const glm::vec3& pos) const {
    return glm::vec3();
}

//bool Sphere::rayInside(const Ray& ray) const {
//    return glm::length(ray.getStart() - _center) <= _radius;
//}

glm::vec3 Sphere::getNormal(glm::vec3 hitpos, glm::vec3 dir) const {
    return (hitpos - _center) / _radius;
}
