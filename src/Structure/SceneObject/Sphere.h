#pragma once
#include "Structure/Object.h"
class Sphere : public Object {
public:
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material);
    ~Sphere() override;

    virtual BoundingBox getBoundingBox() const override;
    virtual bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec3& pos) const override;
    glm::vec3 getSampleColor(const glm::vec3& pos) const  override;
    bool rayInside(const Ray& ray) const override;
    glm::vec3 getNormal(glm::vec3 hitpos, glm::vec3 dir) const override;

private:
    glm::vec3 _center;
    float _radius;
};
