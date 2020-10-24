#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include <vector>

class BruteForce : public AccelStructure {
public:
    BruteForce() = default;
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const override;
    void report() const override;


private:
    std::vector<Object*> _objects;
};
