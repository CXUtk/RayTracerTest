#include "BVH2.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

BVH2::BVH2() :AccelStructure() {
    _nodes[0] = BVH2Node();
    _root = 0, _tot = 0;
    _objects.clear();
}

BVH2::~BVH2() {
}

void BVH2::build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (auto ptr : objects)
        _objects.push_back(ptr.get());
    int sz = _objects.size();
    printf("%d\n", sz);
    _build(_root, 0, sz - 1);
}

static int numStep;
static int totIncs;

bool BVH2::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    numStep = 0;
    totIncs = 0;
    bool ret = false;
    ret |= ray_test(_root, ray, info, 0, info.getDistance());
    _maxWalks = std::max(_maxWalks, (long long)numStep);
    _totWalks += numStep;
    _maxNums = std::max(_maxNums, (long long)totIncs);
    _callCnt++;
    return ret;
}

int BVH2::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    return rayIntersect(ray, info) ? numStep : -1;
}

int BVH2::newNode(Object* objs, const BoundingBox& box, int split) {
    ++_tot;
    _nodes[_tot] = BVH2Node(box, objs, split);
    return _tot;
}

void BVH2::push_up(int p) {
    if (chi(p, 0))
        self.box = self.box.Union(chd(p, 0).box);
    if (chi(p, 1))
        self.box = self.box.Union(chd(p, 1).box);
}

void BVH2::_build(int& p, int l, int r) {
    if (l > r) return;
    if (l == r) {
        p = newNode(_objects[l], _objects[l]->getBoundingBox(), 0);
        return;
    }
    BoundingBox centerBox;
    for (int i = l; i <= r; i++)
        centerBox = centerBox.Union(_objects[i]->getBoundingBox().getCenter());
    int split = centerBox.MaxExtent();
    int mid = (l + r) / 2;
    auto cmp = [=](Object* A, Object* B) {
        return A->getBoundingBox().getCenter()[split] < B->getBoundingBox().getCenter()[split];
    };
    if constexpr (SPLITMETHOD == EQUAL) {
        std::nth_element(_objects.begin() + l, _objects.begin() + mid, _objects.begin() + r + 1, cmp);
    }
    p = newNode(_objects[mid], _objects[mid]->getBoundingBox(), split);
    _build(chi(p, 0), l, mid - 1);
    _build(chi(p, 1), mid + 1, r);
    push_up(p);
}

bool BVH2::ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const {
    numStep++;
    if (!self.box.rayIntersect(ray, tMin, tMax)) return false;
    if (tMin >= info.getDistance()) return false;
    _totNums++;
    totIncs++;
    bool hit = self.objs->rayIntersect(ray, info);
    int d = ray.getDir()[self.splitAxis] < 0 ? 1 : 0;
    if (chi(p, d))
        hit |= ray_test(chi(p, d), ray, info, tMin, info.getDistance());
    if (chi(p, !d))
        hit |= ray_test(chi(p, !d), ray, info, tMin, info.getDistance());
    return hit;
}
