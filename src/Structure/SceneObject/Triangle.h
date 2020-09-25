#pragma once
#include "Structure/Object.h"
#include <vector>
class Triangle : public Object {
public:
    Triangle(const std::vector<glm::vec3>& pts, std::shared_ptr<Material> material);
    Triangle(const glm::vec3 v1, const glm::vec3& v2, const glm::vec3& v3, std::shared_ptr<Material> material);
    ~Triangle() override;

    virtual BoundingBox getBoundingBox() const override;
    virtual bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    virtual glm::vec3 getDiffuseColor(const glm::vec3& pos) const override;
    glm::vec3 getSampleColor(const glm::vec3& pos) const  override;
    glm::vec3 getNormal(glm::vec3 hitpos, glm::vec3 dir) const override;

private:
    glm::vec3 v[3];
    glm::vec3 _normal;
    BoundingBox _boundingBox;
};
