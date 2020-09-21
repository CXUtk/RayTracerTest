#pragma once
#include "Structure/Object.h"
#include "Structure/Utils.h"
#include "Structure/SceneObject/Segment.h"
#include "Structure/BVH/BVH.h"
#include <vector>
class Polygon : public Object {
public:
    Polygon(const std::vector<glm::vec2>& pts, std::shared_ptr<Material> material, const glm::mat3& modelTransform);
    ~Polygon();

    virtual BoundingBox getBoundingBox() const override;
    virtual bool realRayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec2& pos) const override;
    glm::vec3 getSampleColor(const glm::vec2& pos) const  override;
    bool rayInside(const Ray& ray) const override;
    glm::vec2 getNormal(glm::vec2 hitpos, glm::vec2 dir) const override { return glm::vec2(0); }

private:
    // Points must ordered in counter-clockwise direction
    std::vector<glm::vec2> _pts;
    std::vector<std::shared_ptr<Object>> _segments;
    std::unique_ptr<BVHTree> _tree;
};
