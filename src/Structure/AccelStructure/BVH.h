#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include <vector>

struct BVHNode {
    BoundingBox box;
    Object* object;
    int ch[2];

    BVHNode() : box(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity())), object(nullptr) {
        ch[0] = ch[1] = 0;
    }
};
class BVHTree : public AccelStructure {
public:
    BVHTree() = default;
    BVHTree(int maxsize);
    ~BVHTree();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    void walkRectangles(FrameBuffer& frame) const;
    void report() const override;

private:
    int _tot, _root;
    std::vector<BVHNode> _nodes;
    std::vector<Object*> _objects;

    int newNode(Object* optr);
    void push_up(int p);
    void _build(int& p, int l, int r, int d);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, int d) const;

    // Statistic Data
    mutable long long _totNum, _maxNum, _callCnt;
};
