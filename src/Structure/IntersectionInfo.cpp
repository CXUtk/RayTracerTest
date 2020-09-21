#include "IntersectionInfo.h"

IntersectionInfo::IntersectionInfo() :_hitObject(nullptr), _distance(std::numeric_limits<float>::max()), _hitPos(0), _normal(0), _inside(false) {
}

IntersectionInfo::~IntersectionInfo() {
}

void IntersectionInfo::quickSetInfo(const Ray& ray, float t, const Object* obj) {
    auto hitpos = t * ray.getDir() + ray.getStart();
    setHitPos(hitpos);
    setDistance(t);
    setHitObject(obj);
    set_face_normal(ray, obj->getNormal(hitpos, ray.getDir()));
}

void IntersectionInfo::set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
    auto front_face = glm::dot(r.getDir(), outward_normal) < 0;
    _normal = front_face ? outward_normal : -outward_normal;
}
