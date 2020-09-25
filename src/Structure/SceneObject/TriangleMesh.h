#pragma once
#include "Structure/Object.h"
#include "Structure/SceneObject/Triangle.h"
#include "Structure/AccelStructure/AccelStructure.h"
#include <vector>
class TriangleMesh : public Object {
public:
    TriangleMesh() = default;
    TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles);
    BoundingBox getBoundingBox() const override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec3& pos) const override;
    virtual glm::vec3 getSampleColor(const glm::vec3& pos) const override;
    virtual glm::vec3 getNormal(glm::vec3 hitpos, glm::vec3 dir) const override;

private:
    std::vector<std::shared_ptr<Object>> _triangles;
    std::shared_ptr<AccelStructure> _accelStructure;
    BoundingBox _boundingBox;
};
