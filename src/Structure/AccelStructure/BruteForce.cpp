#include "Structure/Utils.h"
#include <algorithm>
#include "BruteForce.h"

void BruteForce::build(const std::vector<std::shared_ptr<Object>>& objects) {
    for (const auto& ptr : objects) {
        _objects.push_back(ptr.get());
    }
}

bool BruteForce::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    bool hit = false;
    for (auto& obj : _objects) {
        IntersectionInfo tmp;
        if (obj->rayIntersect(ray, tmp)) {
            if (tmp.getDistance() < info.getDistance()) info = tmp;
            hit = true;
        }
    }
    return hit;
}

int BruteForce::rayIntersectCount(const Ray& ray, IntersectionInfo& info) const {
    return 0;
}

void BruteForce::report() const {
}
