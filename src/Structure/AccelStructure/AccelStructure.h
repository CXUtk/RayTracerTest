#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Structure/Object.h"
class AccelStructure {
public:
    AccelStructure() = default;
    virtual void build(const std::vector<std::shared_ptr<Object>>& objects) = 0;
    virtual bool rayIntersect(const Ray& ray, IntersectionInfo& info) const = 0;
    virtual void report() const = 0;
};
