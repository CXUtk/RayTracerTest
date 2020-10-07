#include "Triangle.h"
#include "Structure/Utils.h"
Triangle::Triangle(const std::vector<glm::vec3>& pts, std::shared_ptr<Material> material) {
    v[0] = pts[0], v[1] = pts[1], v[2] = pts[2];
    _material = material;

    glm::vec3 minn = v[0];
    glm::vec3 maxx = v[0];
    for (int j = 0; j < 3; j++) {
        minn[j] = std::min(minn[j], v[1][j]);
        maxx[j] = std::max(maxx[j], v[1][j]);
    }
    for (int j = 0; j < 3; j++) {
        minn[j] = std::min(minn[j], v[2][j]);
        maxx[j] = std::max(maxx[j], v[2][j]);
    }
    _boundingBox = BoundingBox(minn, maxx);
}

Triangle::Triangle(const glm::vec3 v1, const glm::vec3& v2, const glm::vec3& v3, std::shared_ptr<Material> material) {
    v[0] = v1, v[1] = v2, v[2] = v3;
    _material = material;

    glm::vec3 minn = v[0];
    glm::vec3 maxx = v[0];
    for (int j = 0; j < 3; j++) {
        minn[j] = std::min(minn[j], v[1][j]);
        maxx[j] = std::max(maxx[j], v[1][j]);
    }
    for (int j = 0; j < 3; j++) {
        minn[j] = std::min(minn[j], v[2][j]);
        maxx[j] = std::max(maxx[j], v[2][j]);
    }
    _boundingBox = BoundingBox(minn, maxx);
}



Triangle::~Triangle() {
}

BoundingBox Triangle::getBoundingBox() const {
    return _boundingBox;
}

bool Triangle::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    glm::mat3 A(v[1] - v[0], v[2] - v[0], -ray.getDir());
    auto det = glm::determinant(A);
    if (!dcmp(det)) return false;
    auto inv = glm::inverse(A);
    glm::vec3 P = ray.getStart() - v[0];
    auto res = inv * P;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
    info.quickSetInfo(ray, res.z, this);
    return true;
}

glm::vec3 Triangle::getDiffuseColor(const glm::vec3& pos) const {
    return _material->getDiffuseColor();
}

glm::vec3 Triangle::getSampleColor(const glm::vec3& pos) const {
    return glm::vec3();
}

//bool Triangle::rayInside(const Ray& ray) const {
//    return false;
//}

glm::vec3 Triangle::getNormal(glm::vec3 hitpos, glm::vec3 dir) const {
    auto N = glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
    auto d = glm::dot(N, dir);
    return d < 0 ? N : -N;
}
