﻿#pragma once
#include "Structure/BoundingBox.h"
#include "AccelStructure.h"
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/Utils.h"
#include <vector>

struct OctNode {
    BoundingBox box;
    std::vector<Object*> objs;
    int ch[8];
    int numOfPrimitives;

    OctNode() : box(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity())), numOfPrimitives(0) {
        memset(ch, 0, sizeof(ch));
    }
};
class Octree : public AccelStructure {
public:
    Octree() = default;
    Octree(int maxsize);
    ~Octree();
    void build(const std::vector<std::shared_ptr<Object>>& objects) override;
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const override;
    void walkRectangles(FrameBuffer& frame) const;
    int numOfNodes() const override { return _tot; }
    void report() const override;
    int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const override;

    void insert(std::shared_ptr<Object> object);

private:
    int _tot, _root;
    OctNode _nodes[MAX_NODES * 4];
    std::vector<Object*> _objects;

    int newNode(const BoundingBox& box);
    void push_up(int p);
    void _insert(int& p, Object* object, int d, const BoundingBox& box);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, int d, float tMin, float tMax)const;
    bool isLeaf(int p) const;

    // Statistic Data
    static constexpr int MAX_DEPTH = 8;
    static constexpr int THRESHOLD = 8;


    enum AllocMethod {
        Dynamic,
        Static,
    };

    static constexpr AllocMethod ALLOC_METHOD = Dynamic;
};
