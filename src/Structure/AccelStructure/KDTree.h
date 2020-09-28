#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include <vector>


enum SplitMethod {
    SAH,
    EQUAL,
};
struct KDTreeNode {
    BoundingBox box;
    std::vector<Object*> objs;
    int ch[2], splitAxis;

    KDTreeNode() : box(), splitAxis(0) {
        ch[0] = ch[1] = 0;
    }

    KDTreeNode(const BoundingBox& box, const std::vector<Object*>& objs, int split) :box(box), objs(objs), splitAxis(split) {
        ch[0] = ch[1] = 0;
    }
};
class KDTree : public AccelStructure {
public:
    KDTree();
    ~KDTree();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    void report() const override;

private:
    int _tot, _root;
    KDTreeNode _nodes[1048576];
    std::vector<Object*> _objects;

    mutable long long _totNum, _maxNum, _callCnt;

    int newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split);
    void push_up(int p);
    void _build(int& p, int l, int r);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const;

    static constexpr int SPLITMETHOD = SplitMethod::SAH;
};
