#include "Circle.h"
Circle::Circle(glm::vec2 center, float radius, std::shared_ptr<Material> material) : _center(center), _radius(radius) {
    _material = material;
}

Circle::~Circle() {
}

BoundingBox Circle::getBoundingBox() const {
    return BoundingBox(_center - glm::vec2(_radius), _center + glm::vec2(_radius));
}

bool Circle::realRayIntersect(const Ray& ray, IntersectionInfo& info) const {
    glm::vec2 diag = _center - ray.getStart();
    float t = glm::dot(diag, ray.getDir());

    if (glm::length(diag) <= _radius) {
        info.setHitObject(this);
        info.setDistance(0);
        info.setHitPos(ray.getStart());
        info.setNormal(getNormal(ray.getStart(), ray.getDir()));
        return true;
    }
    if (t < 0) return false;

    glm::vec2 mid = ray.getStart() + ray.getDir() * t;

    // 投影点连向圆心的向量
    glm::vec2 A = _center - mid;

    float b = _radius * _radius - (A.x * A.x + A.y * A.y);
    if (b < 0) return false;

    float c = t - sqrt(b);
    if (c < 0) c = 0;

    info.setHitObject(this);
    info.setDistance(c);
    info.setHitPos(ray.getStart() + ray.getDir() * c);
    info.setNormal(getNormal(ray.getStart() + ray.getDir() * c, ray.getDir()));
    return true;
}

glm::vec3 Circle::getDiffuseColor(const glm::vec2& pos) const {
    return _material->getDiffuseColor();
}

glm::vec3 Circle::getSampleColor(const glm::vec2& pos) const {
    return _material->getLightColor();
}

bool Circle::rayInside(const Ray& ray) const {
    return glm::distance(ray.getStart(), _center) <= _radius;
}

glm::vec2 Circle::getNormal(glm::vec2 hitpos, glm::vec2 dir) const {
    return glm::normalize(hitpos - _center);
}
