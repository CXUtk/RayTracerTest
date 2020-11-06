#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/Utils.h"
#include <vector>


enum SplitMethod {
    SAH,
    EQUAL,
};
struct BVHSAHNode {
    BoundingBox box;
    std::vector<Object*> objs;
    int ch[2], splitAxis;

    BVHSAHNode() : box(), splitAxis(0) {
        ch[0] = ch[1] = 0;
    }

    BVHSAHNode(const BoundingBox& box, const std::vector<Object*>& objs, int split) :box(box), objs(objs), splitAxis(split) {
        ch[0] = ch[1] = 0;
    }
};
class BVHSAH : public AccelStructure {
public:
    BVHSAH();
    ~BVHSAH();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    void build(const std::vector<Object*>& objects);
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const override;
    int numOfNodes() const override { return _tot; }
    void report() const override;
    int getNodes() const { return _tot; }

private:
    int _tot, _root;
    std::vector<BVHSAHNode> _nodes;
    std::vector<Object*> _objects;

    int newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split);
    void push_up(int p);
    void _build(int& p, int l, int r);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const;

    static constexpr int SPLITMETHOD = SplitMethod::SAH;
};
