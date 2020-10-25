#include "KDTree.h"
#include "Structure/Utils.h"
#include <algorithm>

#define self _nodes[p]
#define chi(p, d) _nodes[p].ch[d]
#define chd(p, d) _nodes[_nodes[p].ch[d]]

struct Edge {
    bool end;
    float pos;
    Edge(bool end, float pos) : end(end), pos(pos) {}
    bool operator<(const Edge& edge) const {
        if (pos == edge.pos)return end < edge.end;
        return pos < edge.pos;
    }
};

KDTree::KDTree() :AccelStructure() {
    _nodes[0] = KDTreeNode();
    _root = 0, _tot = 0;
    _objects.clear();
    _callCnt = 0;
}

KDTree::~KDTree() {
}
static int numStep;
static int totIncs;
void KDTree::build(const std::vector<std::shared_ptr<Object>>& objects) {
    masterBox = BoundingBox();
    for (auto ptr : objects) {
        _objects.push_back(ptr.get());
        masterBox = masterBox.Union(ptr->getBoundingBox());
    }
    int sz = _objects.size();
    _build(_root, masterBox, _objects);
}

bool KDTree::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    numStep = 0;
    totIncs = 0;
    bool ret = false;
    float tMin = 0, tMax = info.getDistance();
    if (!masterBox.rayIntersect(ray, tMin, tMax))return false;
    ret |= ray_test(_root, ray, info, masterBox, tMin, tMax);
    _maxWalks = std::max(_maxWalks, (long long)numStep);
    _totWalks += numStep;
    _maxNums = std::max(_maxNums, (long long)totIncs);
    _callCnt++;
    return ret;
}

int KDTree::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    return rayIntersect(ray, info) ? numStep : -1;
}



int KDTree::newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split, float splitPos) {
    ++_tot;
    _nodes[_tot] = KDTreeNode(objs, split, splitPos);
    return _tot;
}

void KDTree::push_up(int p) {
}

void KDTree::_build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs) {
    if (objs.size() < 4) {
        p = newNode(objs, outerBox, 0, 0);
        return;
    }
    int split = 0;
    float splitPos = 0;
    std::vector<Object*> empty;
    if constexpr (SPLITMETHOD == EQUAL) {
        BoundingBox centerBox;
        for (auto obj : objs)
            centerBox = centerBox.Union(obj->getBoundingBox());
        split = centerBox.MaxExtent();
        if (centerBox.getMaxPos()[split] == centerBox.getMinPos()[split]) {
            p = newNode(objs, outerBox, 0, 0);
            return;
        }
        std::vector<std::pair<float, float>> points;
        for (auto obj : objs) {
            points.push_back({ obj->getBoundingBox().getMinPos()[split], obj->getBoundingBox().getMaxPos()[split] });
        }

        auto cmp = [=](const std::pair<float, float>& A, const std::pair<float, float>& B) {
            if (A.second == B.second)
                return A.first < B.first;
            return A.second < B.second;
        };
        std::sort(points.begin(), points.end(), cmp);
        splitPos = (centerBox.getMaxPos() + centerBox.getMinPos())[split] / 2;
    }
    else {
        float smallestCost = std::numeric_limits<float>::infinity();
        float totalSA = outerBox.SurfaceArea();
        glm::vec3 diff = outerBox.getMaxPos() - outerBox.getMinPos();
        for (int d = 0; d < 3; d++) {
            std::vector<Edge> edges;
            for (auto obj : objs) {
                edges.push_back(Edge(false, obj->getBoundingBox().getMinPos()[d]));
                edges.push_back(Edge(true, obj->getBoundingBox().getMaxPos()[d]));
            }
            int nBelow = 0, nAbove = objs.size();
            float L = outerBox.getMinPos()[d], R = outerBox.getMaxPos()[d];
            sort(edges.begin(), edges.end());
            for (auto& e : edges) {
                if (e.end) --nAbove;
                if (e.pos > L && R) {
                    float LA = diff[(d + 1) % 3];
                    float LB = diff[(d + 2) % 3];
                    float belowSA = 2 * (LA * LB + (e.pos - L) * (LA + LB));
                    float aboveSA = 2 * (LA * LB + (R - e.pos) * (LA + LB));

                    float pb = belowSA / totalSA, pa = aboveSA / totalSA;
                    float cost = 0.125 + (pb * nBelow + pa * nAbove);
                    if (cost < smallestCost) {
                        smallestCost = cost;
                        split = d;
                        splitPos = e.pos;
                    }
                }
                if (!e.end) ++nBelow;
            }
        }
        if (smallestCost >= objs.size()) {
            p = newNode(objs, outerBox, 0, 0);
            return;
        }
    }
    glm::vec3 leftM = outerBox.getMaxPos();
    leftM[split] = splitPos;
    glm::vec3 rightM = outerBox.getMinPos();
    rightM[split] = splitPos;
    BoundingBox boxleft(outerBox.getMinPos(), leftM);
    BoundingBox boxright(rightM, outerBox.getMaxPos());

    std::vector<Object*> leftTri, rightTri;
    for (auto obj : objs) {
        if (obj->getBoundingBox().getMinPos()[split] < splitPos) {
            leftTri.push_back(obj);
        }
        if (obj->getBoundingBox().getMaxPos()[split] > splitPos) {
            rightTri.push_back(obj);
        }
    }
    if (leftTri.size() == objs.size() || rightTri.size() == objs.size()) {
        p = newNode(objs, outerBox, 0, 0);
        return;
    }
    p = newNode(empty, outerBox, split, splitPos);
    _build(chi(p, 0), boxleft, leftTri);
    _build(chi(p, 1), boxright, rightTri);
    push_up(p);
}

bool KDTree::ray_test(int p, const Ray& ray, IntersectionInfo& info, const BoundingBox& outerBox, float tMin, float tMax) const {
    numStep++;
    if (!p || tMin > tMax) return false;
    if (!outerBox.rayIntersect(ray, tMin, tMax)) return false;
    if (tMin >= info.getDistance()) return false;
    bool hit = false;
    if (!self.objs.empty()) {
        totIncs += self.objs.size();
        _totNums += self.objs.size();
        for (auto& obj : self.objs) {
            IntersectionInfo tmp;
            if (obj->rayIntersect(ray, tmp)) {
                if (tmp.getDistance() < info.getDistance()) info = tmp;
                hit = true;
            }
        }
        return hit;
    }
    int split = self.splitAxis;
    float splitPos = self.splitPos;

    glm::vec3 leftM = outerBox.getMaxPos();
    leftM[split] = splitPos;
    glm::vec3 rightM = outerBox.getMinPos();
    rightM[split] = splitPos;

    BoundingBox box[2];
    box[0] = BoundingBox(outerBox.getMinPos(), leftM);
    box[1] = BoundingBox(rightM, outerBox.getMaxPos());
    float tSplit = (splitPos - ray.getStart()[split]) / ray.getDir()[split];


    auto invD = 1.0f / ray.getDir()[split];
    auto t0 = (outerBox.getMinPos()[split] - ray.getStart()[split]) * invD;
    auto t1 = (outerBox.getMaxPos()[split] - ray.getStart()[split]) * invD;
    if (invD < 0) std::swap(t0, t1);
    tMin = std::max(tMin, t0);
    tMax = std::min(tMax, t1);

    int d = ray.getDir()[self.splitAxis] < 0 ? 1 : 0;

    if (tSplit >= tMin) {
        hit |= ray_test(chi(p, d), ray, info, box[d], tMin, tSplit);
    }
    if (tSplit <= tMax) {
        hit |= ray_test(chi(p, !d), ray, info, box[!d], tSplit, tMax);
    }
    return hit;
}
