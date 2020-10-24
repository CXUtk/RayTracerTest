#include "BVHSAH.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

BVHSAH::BVHSAH() :AccelStructure() {
    _nodes[0] = BVHSAHNode();
    _root = 0, _tot = 0;
    _objects.clear();

    _callCnt = 0;
}

BVHSAH::~BVHSAH() {
}

void BVHSAH::build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (auto ptr : objects)
        _objects.push_back(ptr.get());
    int sz = _objects.size();
    printf("%d\n", sz);
    _build(_root, 0, sz - 1);
}

static int numStep;
static int totIncs;

bool BVHSAH::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    numStep = 0;
    totIncs = 0;
    bool hit = false;
    constexpr float MAX = std::numeric_limits<float>::infinity();
    int dirIsNeg[3] = { ray.getDir()[0] < 0, ray.getDir()[1] < 0, ray.getDir()[2] < 0 };

    // Stack related
    int top = 0, p = _root;
    int st[64];
    while (true) {
        numStep++;
        float tmin = 0, tmax = MAX;
        if (self.box.rayIntersect(ray, tmin, tmax) && tmin < info.getDistance()) {
            if (!self.objs.empty()) {
                _totNums += self.objs.size();
                totIncs += self.objs.size();
                for (auto a : self.objs) {
                    if (a->rayIntersect(ray, info)) {
                        hit = true;
                    }
                }
            }
            else {
                int d = 0;
                if (dirIsNeg[self.splitAxis]) d = 1;
                st[top++] = chi(p, !d);
                p = chi(p, d);
                continue;
            }
        }

        if (!top) break;
        p = st[--top];
    }
    _maxWalks = std::max(_maxWalks, (long long)numStep);
    _totWalks += numStep;
    _maxNums = std::max(_maxNums, (long long)totIncs);
    _callCnt++;
    return hit;
}

int BVHSAH::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    return rayIntersect(ray, info) ? numStep : -1;
}


int BVHSAH::newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split) {
    ++_tot;
    _nodes[_tot] = BVHSAHNode(box, objs, split);
    return _tot;
}

void BVHSAH::push_up(int p) {
    if (chi(p, 0))
        self.box = self.box.Union(chd(p, 0).box);
    if (chi(p, 1))
        self.box = self.box.Union(chd(p, 1).box);
}

void BVHSAH::_build(int& p, int l, int r) {
    BoundingBox box;
    std::vector<Object*> objs;
    if (l > r - 6) {
        for (int i = l; i <= r; i++) {
            box = box.Union(_objects[i]->getBoundingBox());
            objs.push_back(_objects[i]);
        }
        p = newNode(objs, box, 0);
        return;
    }
    BoundingBox centerBox;
    for (int i = l; i <= r; i++)
        centerBox = centerBox.Union(_objects[i]->getBoundingBox().getCenter());
    int split = centerBox.MaxExtent();
    if (centerBox.getMaxPos()[split] == centerBox.getMinPos()[split]) {
        for (int i = l; i <= r; i++) {
            box = box.Union(_objects[i]->getBoundingBox());
            objs.push_back(_objects[i]);
        }
        p = newNode(objs, box, 0);
        return;
    }
    int mid = (l + r) / 2;
    auto cmp = [=](Object* A, Object* B) {
        return A->getBoundingBox().getCenter()[split] < B->getBoundingBox().getCenter()[split];
    };
    if constexpr (SPLITMETHOD == EQUAL) {
        std::nth_element(_objects.begin() + l, _objects.begin() + mid, _objects.begin() + r + 1, cmp);
    }
    else {
        constexpr float tTrav = 0.125;
        sort(_objects.begin() + l, _objects.begin() + r + 1, cmp);
        BoundingBox* suf = new BoundingBox[r - l + 2];
        suf[r - l + 1] = BoundingBox();
        float cost = std::numeric_limits<float>::infinity();
        int mincost = -1;
        for (int i = r - l; i >= 0; i--) {
            suf[i] = suf[i + 1].Union(_objects[i + l]->getBoundingBox());
        }
        BoundingBox curbox = BoundingBox();
        for (int i = 0; i < r - l + 1; i++) {
            curbox = curbox.Union(_objects[i + l]->getBoundingBox());
            float lArea = curbox.SurfaceArea();
            float rArea = suf[i].SurfaceArea();
            float c = tTrav + ((i + 1) * lArea + (r - l - i) * rArea) / centerBox.SurfaceArea();
            if (c < cost) {
                cost = c;
                mincost = i;
            }
        }
        delete[] suf;
        mid = l + mincost;
        if (mincost > r - l + 1) {
            for (int i = l; i <= r; i++) {
                box = box.Union(_objects[i]->getBoundingBox());
                objs.push_back(_objects[i]);
            }
            p = newNode(objs, box, 0);
            return;
        }
    }
    p = newNode(objs, box, split);
    _build(chi(p, 0), l, mid);
    _build(chi(p, 1), mid + 1, r);
    push_up(p);
}

bool BVHSAH::ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const {
    numStep++;
    if (!self.box.rayIntersect(ray, tMin, tMax)) return false;
    if (tMin >= info.getDistance()) return false;
    bool hit = false;
    if (!self.objs.empty()) {
        IntersectionInfo tmp;
        for (auto a : self.objs) {
            if (a->rayIntersect(ray, info)) {
                hit = true;
            }
        }
    }
    int d = ray.getDir()[self.splitAxis] < 0 ? 1 : 0;
    if (chi(p, d))
        hit |= ray_test(chi(p, d), ray, info, tMin, info.getDistance());
    if (chi(p, !d))
        hit |= ray_test(chi(p, !d), ray, info, tMin, info.getDistance());
    return hit;
}
