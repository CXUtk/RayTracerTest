#include "BVHSAH.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

BVHSAH::BVHSAH() {
    _nodes[0] = BVHSAHNode();
    _root = 0, _tot = 0;
    _objects.clear();
    _totNum = 0;
    _maxNum = 0;
    _callCnt = 0;
}

BVHSAH::~BVHSAH() {
}

void BVHSAH::build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (auto ptr : objects) {
        _objects.push_back(ptr.get());
    }
    int sz = _objects.size();
    _build(_root, 0, sz - 1, 0);
}

static int numStep;
bool BVHSAH::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    float t;
    numStep = 0;
    bool ret = false;
    ret |= _nodes[_root].box.rayIntersect(ray, t) && ray_test(_root, ray, info, 0);
    _totNum += numStep;
    _maxNum = std::max(_maxNum, (long long)numStep);
    _callCnt++;
    return ret;
}
void BVHSAH::report() const {
    printf("Total Step: %lld\n", _totNum);
    printf("Total Called: %lld\n", _callCnt);
    printf("Average Step: %lf\n", _totNum / (double)_callCnt);
    printf("Max Step: %lld\n", _maxNum);
}


int BVHSAH::newNode(Object* optr) {
    ++_tot;
    _nodes[_tot] = BVHSAHNode();
    _nodes[_tot].box = optr->getBoundingBox();
    // _nodes[_tot].object = optr;
    return _tot;
}

void BVHSAH::push_up(int p) {
    self.box = self.box.Union(chd(p, 0).box);
    self.box = self.box.Union(chd(p, 1).box);
}

void BVHSAH::_build(int& p, int l, int r, int d) {
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

bool BVHSAH::ray_test(int p, const Ray& ray, IntersectionInfo& info, int d) const {
    //numStep++;
    //if (!p) return false;
    //float tl, tr;
    //bool hit = false;
    //if (self.object != nullptr) {
    //    IntersectionInfo tmp;
    //    if (self.object->rayIntersect(ray, tmp)) {
    //        if (tmp.getDistance() < info.getDistance()) {
    //            info = tmp;
    //        }
    //        hit = true;
    //    }
    //}
    //int nxt = (d + 1) % 3;
    //if (!chi(p, 0) || !chd(p, 0).box.rayIntersect(ray, tl)) tl = std::numeric_limits<float>::max();
    //if (!chi(p, 1) || !chd(p, 1).box.rayIntersect(ray, tr)) tr = std::numeric_limits<float>::max();
    //int d2 = 0;
    //if (tl > tr) {
    //    std::swap(tl, tr);
    //    d2 = 1;
    //}
    //if (tl == std::numeric_limits<float>::max()) return hit;
    //if (tl < info.getDistance()) {
    //    hit |= ray_test(chi(p, d2), ray, info, nxt);
    //}
    //if (tr < info.getDistance()) {
    //    hit |= ray_test(chi(p, !d2), ray, info, nxt);
    //}
    return false;
}
