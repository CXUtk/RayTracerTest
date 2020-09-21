#pragma once
#include "Structure/Object.h"



class Segment : public Object {
public:
    Segment(glm::vec2 start, glm::vec2 end) :_start(start), _end(end) {}

    virtual BoundingBox getBoundingBox() const override;
    virtual bool realRayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec2& pos) const override { return _material->getDiffuseColor(); }
    glm::vec3 getSampleColor(const glm::vec2& pos) const  override { return _material->getLightColor(); }
    bool rayInside(const Ray& ray) const override;
    glm::vec2 getNormal(glm::vec2 hitpos, glm::vec2 dir) const override;

private:
    glm::vec2 _start, _end;
};
