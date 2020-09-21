#pragma once
#include <memory>
#include "Object.h"
#include "Ray.h"
class Object;
class IntersectionInfo {
public:
    IntersectionInfo();
    ~IntersectionInfo();

    void setHitObject(const Object* object) { _hitObject = object; }
    void setDistance(float distance) { _distance = distance; }
    void setHitPos(glm::vec3 hitpos) { _hitPos = hitpos; }
    void setNormal(glm::vec3 normal) { _normal = normal; }
    void setInside(bool value) { _inside = value; }
    void quickSetInfo(const Ray& ray, float t, const Object* obj);


    bool isInside() const { return _inside; }
    float getDistance() const { return _distance; }
    const Object* getHitObject() const { return _hitObject; }
    glm::vec3 getHitPos() const { return _hitPos; }
    glm::vec3 getNormal() const { return _normal; }


private:
    const Object* _hitObject;
    float _distance;
    bool _inside;
    glm::vec3 _hitPos;
    glm::vec3 _normal;

    void set_face_normal(const Ray& r, const glm::vec3& outward_normal);
};
