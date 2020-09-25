#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include <vector>


constexpr int MAX_PRIMES_IN_NODE = 20;
struct BVHSAHNode {
    BoundingBox box;
    Object* objects[MAX_PRIMES_IN_NODE];
    int ch[2];

    BVHSAHNode() :box(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity())) {
        memset(objects, 0, sizeof(objects));
        ch[0] = ch[1] = 0;
    }
};
class BVHSAH : public AccelStructure {
public:
    BVHSAH();
    ~BVHSAH();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    void report() const override;

private:
    int _tot, _root;
    BVHSAHNode _nodes[1048576];
    std::vector<Object*> _objects;

    mutable long long _totNum, _maxNum, _callCnt;

    int newNode(Object* optr);
    void push_up(int p);
    void _build(int& p, int l, int r, int d);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, int d) const;
};
