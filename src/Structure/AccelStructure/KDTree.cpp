﻿#include "KDTree.h"
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
        if (pos == edge.pos)return end > edge.end;
        return pos < edge.pos;
    }
};

KDTree::KDTree() :AccelStructure() {
    _nodes[0] = KDTreeNode();
    _root = 0, _tot = 0;
    _objects.clear();
}

KDTree::~KDTree() {
}

void KDTree::build(const std::vector<std::shared_ptr<Object>>& objects) {
    masterBox = BoundingBox();
    for (auto ptr : objects) {
        _objects.push_back(ptr.get());
        masterBox = masterBox.Union(ptr->getBoundingBox());
    }
    int sz = _objects.size();
    _build(_root, masterBox, _objects, 0);
}

bool KDTree::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    _numStep = 0;
    _totIncs = 0;
    bool ret = false;
    float tMin = 0, tMax = info.getDistance();
    if (!masterBox.rayIntersect(ray, tMin, tMax))return false;
    ret |= ray_test(_root, ray, info, tMin, tMax);
    _maxWalks = std::max(_maxWalks, _numStep);
    _totWalks += _numStep;
    _maxNums = std::max(_maxNums, _totIncs);
    _callCnt++;
    return ret;
}

int KDTree::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    rayIntersect(ray, info);
    return _numStep;
}

void KDTree::report() const {
    AccelStructure::report();
    printf("Total # of nodes: %d\n", _tot);
    printf("Total # of nodes + inc: %d\n", _totMemory);
}



int KDTree::newNode(const std::vector<Object*>& objs, const BoundingBox& box, int split, float splitPos) {
    ++_tot;
    _totMemory++;
    _nodes[_tot] = KDTreeNode(objs, split, splitPos);
    return _tot;
}

void KDTree::push_up(int p) {
}

void KDTree::_build(int& p, const BoundingBox& outerBox, std::vector<Object*>& objs, int depth) {
    if (objs.size() < 8 || depth == MAX_DEPTH) {
        _totMemory += objs.size();
        p = newNode(objs, outerBox, -1, -1);
        return;
    }
    int split = 0;
    float splitPos = 0;
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
        for (int i = 0; i < edges.size(); i++) {
            Edge& e = edges[i];
            if (e.end) --nAbove;
            if (e.pos > L && e.pos < R) {
                float LA = diff[(d + 1) % 3];
                float LB = diff[(d + 2) % 3];
                float belowSA = 2 * (LA * LB + (e.pos - L) * (LA + LB));
                float aboveSA = 2 * (LA * LB + (R - e.pos) * (LA + LB));
                float eb = (nAbove == 0 || nBelow == 0) ? 0.2 : 0;
                float pb = belowSA / totalSA, pa = aboveSA / totalSA;
                float cost = 0.05 + (1 - eb) * (pb * nBelow + pa * nAbove);
                if (cost < smallestCost) {
                    smallestCost = cost;
                    split = d;
                    splitPos = e.pos;
                }
            }
            if (!e.end) ++nBelow;
        }
    }
    if (smallestCost >= objs.size() * 2) {
        _totMemory += objs.size();
        p = newNode(objs, outerBox, -1, -1);
        return;
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

    p = newNode(objs, outerBox, split, splitPos);
    _build(chi(p, 0), boxleft, leftTri, depth + 1);
    _build(chi(p, 1), boxright, rightTri, depth + 1);
    push_up(p);
}

bool KDTree::ray_test(int p, const Ray& ray, IntersectionInfo& info, float tMin, float tMax) const {
    _numStep++;
    if (!p || tMin > tMax) return false;
    //if (!outerBox.rayIntersect(ray, tMin, tMax)) return false;
    if (tMin >= info.getDistance()) return false;
    if (self.splitAxis == -1) {
        bool hit = false;
        _totNums += self.objs.size();
        _totIncs += self.objs.size();
        for (auto obj : self.objs) {
            IntersectionInfo tmp;
            if (obj->rayIntersect(ray, tmp)) {
                if (tmp.getDistance() < info.getDistance()) {
                    info = tmp;
                }
                hit = true;
            }
        }
        return hit;
    }
    int split = self.splitAxis;
    float splitPos = self.splitPos;
    float tSplit = (splitPos - ray.getStart()[split]) / ray.getDir()[split];
    bool hit = false;
    int d = ray.getDir()[split] < 0;
    if (tSplit >= tMin) {
        hit |= ray_test(chi(p, d), ray, info, tMin, std::min({ tSplit, tMax, info.getDistance() }));
    }
    if (tSplit <= tMax) {
        hit |= ray_test(chi(p, !d), ray, info, std::max(tSplit, 0.0f), std::min(tMax, info.getDistance()));
    }
    return hit;
}
