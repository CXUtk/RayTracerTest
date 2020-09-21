#include "Polygon.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
Polygon::Polygon(const std::vector<glm::vec2>& pts, std::shared_ptr<Material> material, const glm::mat3& modelTransform) {
    _material = material;
    for (auto v : pts) {
        _pts.push_back(modelTransform * glm::vec3(v, 1));
    }
    int sz = _pts.size();
    auto normalTrans = glm::transpose(glm::inverse(modelTransform));
    for (int i = 0; i < sz; i++) {
        auto p = std::make_shared<Segment>(_pts[(i + 1) % sz], _pts[i]);
        p->setMaterial(material);
        _segments.push_back(p);
    }
    _tree = std::make_unique<BVHTree>();
    _tree->build(_segments);
}

Polygon::~Polygon() {
}

BoundingBox Polygon::getBoundingBox() const {
    int sz = _pts.size();
    glm::vec2 maxx(_pts[0]), minn(_pts[0]);
    for (int i = 1; i < sz; i++) {
        maxx.x = std::max(maxx.x, _pts[i].x);
        maxx.y = std::max(maxx.y, _pts[i].y);
        minn.x = std::min(minn.x, _pts[i].x);
        minn.y = std::min(minn.y, _pts[i].y);
    }
    return BoundingBox(minn, maxx);
}

bool Polygon::realRayIntersect(const Ray& ray, IntersectionInfo& info) const {
#ifdef BRUTE
    float finalT = std::numeric_limits<float>::max();
    bool can = false;
    for (auto& seg : _segments) {
        IntersectionInfo tmp;
        if (seg->realRayIntersect(ray, tmp)) {
            if (tmp.getDistance() < finalT) {
                info.setHitObject(this);
                info.setDistance(tmp.getDistance());
                info.setHitPos(tmp.getHitPos());
                info.setNormal(seg->getNormal(tmp.getHitPos(), ray.getDir()));
                finalT = tmp.getDistance();
            }
            can = true;
        }
    }
    return can;
#else
    if (!_tree->rayIntersect(ray, info))return false;
    info.setNormal(info.getHitObject()->getNormal(info.getHitPos(), ray.getDir()));
    info.setHitObject(this);
    return true;
#endif // BRUTE
}

glm::vec3 Polygon::getDiffuseColor(const glm::vec2& pos) const {
    return _material->getDiffuseColor();
}

glm::vec3 Polygon::getSampleColor(const glm::vec2& pos) const {
    return _material->getLightColor();
}

bool Polygon::rayInside(const Ray& ray) const {
    int sz = _pts.size();
    auto point = ray.getStart();
    for (int i = 0; i < sz; i++) {
        glm::vec2 dir = _pts[(i + 1) % sz] - _pts[i];
        if (cross2(dir, point - _pts[i]) < 0)return false;
    }
    return true;
}
