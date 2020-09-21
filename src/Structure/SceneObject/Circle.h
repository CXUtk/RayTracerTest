#pragma once
#include "Structure/Object.h"
class Circle : public Object {
public:
    Circle(glm::vec2 center, float radius, std::shared_ptr<Material> material);
    ~Circle() override;

    virtual BoundingBox getBoundingBox() const override;
    virtual bool realRayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec2& pos) const override;
    glm::vec3 getSampleColor(const glm::vec2& pos) const  override;
    bool rayInside(const Ray& ray) const override;
    glm::vec2 getNormal(glm::vec2 hitpos, glm::vec2 dir) const override;

private:
    glm::vec2 _center;
    float _radius;
};
