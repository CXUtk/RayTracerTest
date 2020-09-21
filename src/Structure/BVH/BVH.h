#pragma once
#include "Structure/BoundingBox.h"
#include "Structure/Object.h"
#include "Structure/Ray.h"
#include "Structure/IntersectionInfo.h"
#include "Render/FrameBuffer.h"
#include <vector>
struct BVHNode {
    BoundingBox box;
    Object* object;
    int ch[2];

    BVHNode() :box(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity())), object(nullptr) { ch[0] = ch[1] = 0; }
};
class BVHTree {
public:
    BVHTree();
    ~BVHTree();
    void build(const std::vector<std::shared_ptr<Object>>& objects);
    bool rayIntersect(const Ray& ray, IntersectionInfo& info) const;
    void walkRectangles(FrameBuffer& frame) const;
private:
    int _tot, _root;
    BVHNode _nodes[1048576];
    std::vector<Object*> _objects;

    void _dfsDrawRect(int p, FrameBuffer& frame) const;


    int newNode(Object* optr);
    void push_up(int p);
    void _build(int& p, int l, int r, int d);
    bool ray_test(int p, const Ray& ray, IntersectionInfo& info, int d) const;
};
