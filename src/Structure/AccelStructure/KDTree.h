﻿#pragma once
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

    KDTreeNode() : splitAxis(-1), splitPos(-1) {
        ch[0] = ch[1] = 0;
    }

    KDTreeNode(const std::vector<Object*>& objs, int split, float splitPos) : splitAxis(split), splitPos(splitPos) {
        ch[0] = ch[1] = 0;
        if (split == -1) {
            this->objs = objs;
        }
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
    void report() const override;

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
    void _build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs, int depth);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const;

    static constexpr int MAX_DEPTH = 25;
};
