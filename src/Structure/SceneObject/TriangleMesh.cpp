#include "TriangleMesh.h"
#include "Structure/AccelStructure/Quick.h"
TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Triangle>>& triangles) {
    for (auto t : triangles) {
        _triangles.push_back(t);
        _boundingBox = _boundingBox.Union(t->getBoundingBox());
    }
    _accelStructure = std::make_shared<Quick>(triangles.size());
    _accelStructure->build(_triangles);
}

BoundingBox TriangleMesh::getBoundingBox() const {
    return _boundingBox;
}

bool TriangleMesh::rayIntersect(const Ray& ray, IntersectionInfo& info) const {
    return _accelStructure->rayIntersect(ray, info);
}

glm::vec3 TriangleMesh::getDiffuseColor(const glm::vec3& pos) const {
    return glm::vec3();
}

glm::vec3 TriangleMesh::getSampleColor(const glm::vec3& pos) const {
    return glm::vec3();
}

glm::vec3 TriangleMesh::getNormal(glm::vec3 hitpos, glm::vec3 dir) const {
    return glm::vec3();
}
