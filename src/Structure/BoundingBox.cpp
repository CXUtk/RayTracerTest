#include "BoundingBox.h"
#include "Utils.h"
BoundingBox::BoundingBox() : _minPos(glm::vec3(std::numeric_limits<float>::infinity())), _maxPos(glm::vec3(-std::numeric_limits<float>::infinity())) {
}
BoundingBox::BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos) : _minPos(minPos), _maxPos(maxPos) {
}

BoundingBox::~BoundingBox() {
}

bool BoundingBox::rayIntersect(const Ray& ray, float& tMin, float& tMax) const {
    glm::vec3 start = ray.getStart(), dir = ray.getDir();
    for (int i = 0; i < 3; i++) {
        auto invD = 1.0f / dir[i];
        auto t0 = (_minPos[i] - start[i]) * invD;
        auto t1 = (_maxPos[i] - start[i]) * invD;
        if (invD < 0) std::swap(t0, t1);
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
        if (tMax < tMin) return false;
    }
    //float tRange[3][2];
    //// 竖直的射线
    //if (!dcmp(ray.getDir().x)) {
    //    if (ray.getStart().x < _minPos.x || ray.getStart().x > _maxPos.x) {
    //        return false;
    //    }
    //}
    //// 横向的
    //if (!dcmp(ray.getDir().y)) {
    //    if (ray.getStart().y < _minPos.y || ray.getStart().y > _maxPos.y) {
    //        return false;
    //    }
    //}
    //tRange[0][0] = 0, tRange[0][1] = std::numeric_limits<float>::max();
    //tRange[1][0] = 0, tRange[1][1] = std::numeric_limits<float>::max();
    //tRange[2][0] = 0, tRange[2][1] = std::numeric_limits<float>::max();
    //glm::vec3 pos[2];
    //// j代表第几维
    //for (int j = 0; j < 3; j++) {
    //    // 如果是合法不变的xy值，那么这一维将不参与判定
    //    if (!dcmp(ray.getDir().x) && !j) continue;
    //    if (!dcmp(ray.getDir().y) && j) continue;
    //    pos[0] = _minPos, pos[1] = _maxPos;
    //    if (ray.getDir()[j] < 0) std::swap(pos[0], pos[1]);
    //    // i代表最小，最大
    //    for (int i = 0; i < 2; i++) {
    //        tRange[j][i] = (pos[i][j] - ray.getStart()[j]) / ray.getDir()[j];
    //    }
    //}
    //float tMin = std::max({ tRange[0][0], tRange[1][0], tRange[2][0] }), tMax = std::min({ tRange[0][1], tRange[1][1], tRange[2][1] });
    //if (tMin > tMax) return false;
    //if (std::max(tMin, tMax) < 0) return false;
    //tNear = std::max(0.f, tMin);
    return true;
}

bool BoundingBox::intersects(const BoundingBox& box) const {
    for (int i = 0; i < 3; i++)
        if (std::max(_minPos[i], box.getMinPos()[i]) > std::min(_maxPos[i], box.getMaxPos()[i])) return false;
    return true;
}
