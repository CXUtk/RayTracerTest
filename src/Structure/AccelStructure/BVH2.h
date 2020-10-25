#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/Utils.h"
#include <vector>



struct BVH2Node {

    BoundingBox box;
    Object* objs;
    int ch[2], splitAxis;

    BVH2Node() : box(), splitAxis(0) {
        ch[0] = ch[1] = 0;
    }

    BVH2Node(const BoundingBox& box, Object* objs, int split) :box(box), objs(objs), splitAxis(split) {
        ch[0] = ch[1] = 0;
    }
};
class BVH2 : public AccelStructure {
public:

    BVH2();
    ~BVH2();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    int numOfNodes() const override { return _tot; }
    int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const override;

private:
    int _tot, _root;
    BVH2Node _nodes[MAX_NODES];
    std::vector<Object*> _objects;


    int newNode(Object* objs, const BoundingBox& box, int split);
    void push_up(int p);
    void _build(int& p, int l, int r);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const;

    enum SplitMethod {
        SAH,
        EQUAL,
    };
    static constexpr int SPLITMETHOD = SplitMethod::EQUAL;
};
