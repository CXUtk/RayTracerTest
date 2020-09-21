#include "Segment.h"
#include "Structure/Utils.h"

BoundingBox Segment::getBoundingBox() const {
    glm::vec2 minn, maxx;
    minn = _start, maxx = _start;
    minn.x = std::min(minn.x, _end.x);
    minn.y = std::min(minn.y, _end.y);
    maxx.x = std::max(maxx.x, _end.x);
    maxx.y = std::max(maxx.y, _end.y);
    return BoundingBox(minn, maxx);
}


bool Segment::realRayIntersect(const Ray& ray, IntersectionInfo& info) const {
    //if (!dcmp(cross2(ray.getDir(), _end - _start))) {
    //    if (glm::dot(ray.getStart() - _start, _end - _start) >= 0) {
    //        info.setInside(true);
    //        info.quickSetInfo(ray, 0, this);
    //        return true;
    //    }
    //}
    float d;
    if (raySegmentIntersect(ray, _start, _end, d)) {
        info.quickSetInfo(ray, d, this);
        return true;
    }
    return false;

    //glm::vec2 dir = glm::normalize(_end - _start);
    //float crs = cross2(ray.getDir(), dir);
    // 如果平行
    //if (std::abs(crs) < EPS) {
    //     如果不在射线上
    //    if (glm::dot(_end - ray.getStart(), ray.getDir()) < 0) return false;
    //     如果两个线是重叠的
    //    if (std::abs(cross2(_end - ray.getStart(), dir)) < EPS) {
    //         如果点在线段上
    //        if (glm::dot(ray.getStart() - _start, dir) >= 0) {
    //            info.setInside(true);
    //            info.quickSetInfo(ray, 0, this);
    //            return true;
    //        }
    //         否则选一个最近端点
    //        float d1 = glm::distance(ray.getStart(), _start);
    //        float d2 = glm::distance(ray.getStart(), _end);
    //        info.quickSetInfo(ray, std::min(d1, d2), this);
    //        return true;
    //    }
    //    return false;
    //}

    //auto v = lineIntersection(ray.getStart(), ray.getDir(), _start, dir);
    //float d = glm::dot(ray.getDir(), v - ray.getStart());
    // 如果不在射线上
    //if (d < 0) return false;

    // 如果不在线段上
    //if (glm::dot(dir, v - _start) < 0 || glm::distance(v, _start) > glm::distance(_start, _end)) return false;
    //info.quickSetInfo(ray, d, this);
    //return true;
}

bool Segment::rayInside(const Ray& ray) const {
    // cross product must be 0 so that ray lies on the line
    if (dcmp(cross2(_end - _start, ray.getStart() - _start))) return false;
    return dcmp(glm::dot(_end - _start, ray.getStart() - _start)) > 0 && dcmp(glm::dot(_end - _start, ray.getStart() - _end)) < 0;
}

glm::vec2 Segment::getNormal(glm::vec2 hitpos, glm::vec2 dir) const {
    glm::vec2 line = glm::normalize(_end - _start);
    float r = glm::half_pi<float>();
    if (cross2(line, dir) > 0) {
        r *= -1;
    }
    return glm::rotate(line, r);
}
