#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/Utils.h"
#include <vector>


struct KDTreeNode {
    std::vector<Object*> objs;
    int ch[2], splitAxis;
    float splitPos;

    KDTreeNode() : splitAxis(0), splitPos(0) {
        ch[0] = ch[1] = 0;

    }

    KDTreeNode(const std::vector<Object*>& objs, int split, float splitPos) : objs(objs), splitAxis(split), splitPos(splitPos) {
        ch[0] = ch[1] = 0;
    }
};
class KDTree : public AccelStructure {
public:
    KDTree();
    ~KDTree();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const override;
    int numOfNodes() const override { return _tot; }

private:
    enum SplitMethod {
        SAH,
        EQUAL,
    };
    int _tot, _root;
    KDTreeNode _nodes[MAX_NODES];
    std::vector<Object*> _objects;
    BoundingBox masterBox;



    int newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split, float splitPos);
    void push_up(int p);
    void _build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, const BoundingBox& outerBox, float tMin, float tMax) const;

    static constexpr int SPLITMETHOD = SplitMethod::SAH;
};
