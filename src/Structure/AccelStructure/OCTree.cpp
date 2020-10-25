#include "OCTree.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

Octree::Octree(int maxsize) :AccelStructure() {
    _root = 0, _tot = 0;
    _objects.clear();
}

Octree::~Octree() {
}

void Octree::build(const std::vector<std::shared_ptr<Object>>& objects) {
    BoundingBox box;
    for (const auto& ptr : objects) {
        box = box.Union(ptr->getBoundingBox());
    }
    for (const auto& ptr : objects) {
        _objects.push_back(ptr.get());
        _insert(_root, ptr, 0, box);
    }
}

static int numStep;
static int totIncs;
bool Octree::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    numStep = 0;
    totIncs = 0;
    bool ret = false;
    ret |= ray_test(_root, ray, info, 0, 0, info.getDistance());
    _maxWalks = std::max(_maxWalks, (long long)numStep);
    _totWalks += numStep;
    _maxNums = std::max(_maxNums, (long long)totIncs);
    _callCnt++;
    return ret;
}

void Octree::walkRectangles(FrameBuffer& frame) const {
    // _dfsDrawRect(_root, frame);
}

int Octree::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    return rayIntersect(ray, info) ? numStep : -1;
}

void Octree::insert(std::shared_ptr<Object> object) {
}

//void Quick::_dfsDrawRect(int p, FrameBuffer& frame) const {
//    if (!p) return;
//    if (self.object != nullptr) {
//        auto box = self.object->getBoundingBox();
//        drawRectangle(box.getMinPos(), box.getMaxPos() - box.getMinPos(), frame);
//    }
//    drawRectangle(self.box.getMinPos(), self.box.getMaxPos() - self.box.getMinPos(), frame);
//    _dfsDrawRect(chi(p, 0), frame);
//    _dfsDrawRect(chi(p, 1), frame);
//}

int Octree::newNode(const BoundingBox& box) {
    ++_tot;
    _nodes[_tot].box = box;
    return _tot;
}

void Octree::push_up(int p) {
    for (int i = 0; i < 8; i++)
        self.numOfPrimitives += chd(p, i).numOfPrimitives;
}

void Octree::_insert(int& p, std::shared_ptr<Object> object, int d, const BoundingBox& box) {
    if (!p) p = newNode(box);
    if (d == MAX_DEPTH) {
        self.objs.push_back(object.get());
        return;
    }
    if (d < MAX_DEPTH && !chi(p, 0) && self.objs.size() + 1 <= THRESHOLD) {
        self.objs.push_back(object.get());
    }
    else if (d < MAX_DEPTH)
        glm::vec3 halfV = (self.box.getMaxPos() - self.box.getMinPos()) / 2.f;
    for (int i = 0; i < 8; i++) {
        auto nxtMin = box.getMinPos();
        auto nxtMax = nxtMin + halfV;
        for (int j = 0; j < 3; j++) {
            if ((i >> j) & 1) {
                nxtMin[j] += halfV[j];
                nxtMax[j] = box.getMaxPos()[j];
            }
        }
        auto nxtBox = BoundingBox(nxtMin, nxtMax);
        if (object->getBoundingBox().intersects(nxtBox)) {
            _insert(chi(p, i), object, d + 1, nxtBox);
        }
    }
    push_up(p);
}


bool Octree::ray_test(int p, const Ray& ray, IntersectionInfo& info, int d, float tMin, float tMax) const {
    numStep++;
    if (!self.box.rayIntersect(ray, tMin, tMax)) return false;
    bool hit = false;
    if (d == MAX_DEPTH) {
        totIncs += self.objs.size();
        _totNums += self.objs.size();
        for (auto o : self.objs) {
            IntersectionInfo tmp;
            if (o->rayIntersect(ray, tmp)) {
                if (tmp.getDistance() < info.getDistance()) {
                    info = tmp;
                }
                hit = true;
            }
        }
        return hit;
    }
    for (int i = 0; i < 8; i++) {
        if (chi(p, i))
            hit |= ray_test(chi(p, i), ray, info, d + 1, tMin, info.getDistance());
    }
    return hit;
}
