#include "BVH.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

BVHTree::BVHTree() {
    _nodes[0] = BVHNode();
    _root = 0, _tot = 0;
    _objects.clear();
}

BVHTree::~BVHTree() {
}

void BVHTree::build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (auto ptr : objects) {
        _objects.push_back(ptr.get());
    }
    int sz = _objects.size();
    _build(_root, 0, sz - 1, 0);
}

bool BVHTree::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    float t;
    return _nodes[_root].box.rayIntersect(ray, t) && ray_test(_root, ray, info, 0);
}

void BVHTree::walkRectangles(FrameBuffer& frame) const {
    _dfsDrawRect(_root, frame);
}

void BVHTree::_dfsDrawRect(int p, FrameBuffer& frame) const {
    if (!p) return;
    if (self.object != nullptr) {
        auto box = self.object->getBoundingBox();
        drawRectangle(box.getMinPos(), box.getMaxPos() - box.getMinPos(), frame);
    }
    drawRectangle(self.box.getMinPos(), self.box.getMaxPos() - self.box.getMinPos(), frame);
    _dfsDrawRect(chi(p, 0), frame);
    _dfsDrawRect(chi(p, 1), frame);
}

int BVHTree::newNode(Object* optr) {
    ++_tot;
    _nodes[_tot] = BVHNode();
    _nodes[_tot].box = optr->getBoundingBox();
    _nodes[_tot].object = optr;
    return _tot;
}

void BVHTree::push_up(int p) {
    self.box = self.box.Union(chd(p, 0).box);
    self.box = self.box.Union(chd(p, 1).box);
}

void BVHTree::_build(int& p, int l, int r, int d) {
    if (l > r) return;
    int mid = (l + r) / 2;
    auto cmp = [=](Object* A, Object* B) {
        return A->getBoundingBox().getCenter()[d] < B->getBoundingBox().getCenter()[d];
    };
    std::nth_element(_objects.begin() + l, _objects.begin() + mid, _objects.begin() + r, cmp);
    if (!p) p = newNode(_objects[mid]);
    _build(chi(p, 0), l, mid - 1, (d + 1) % 3);
    _build(chi(p, 1), mid + 1, r, (d + 1) % 3);
    push_up(p);
}

bool BVHTree::ray_test(int p, const Ray& ray, IntersectionInfo& info, int d) const {
    float t;
    bool hit = false;
    if (self.object != nullptr) {
        IntersectionInfo tmp;
        if (self.object->rayInside(ray)) {
            info.setHitObject(self.object);
            info.setDistance(0);
            info.setHitPos(ray.getStart());
            info.setInside(true);
            return true;
        }
        if (self.object->rayIntersect(ray, tmp)) {
            if (tmp.getDistance() < info.getDistance()) {
                info = tmp;
            }
            hit = true;
        }
    }
    if (chi(p, 0) && chd(p, 0).box.rayIntersect(ray, t) && t < info.getDistance()) {
        hit |= ray_test(chi(p, 0), ray, info, !d);
    }
    if (chi(p, 1) && chd(p, 1).box.rayIntersect(ray, t) && t < info.getDistance()) {
        hit |= ray_test(chi(p, 1), ray, info, !d);
    }
    return hit;
}
