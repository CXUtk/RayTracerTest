#include "Camera.h"

Camera::Camera(const glm::vec3& pos, float fov, float aspectRatio, float zNear) :_pos(pos), _fov(fov), _aspectRatio(aspectRatio), _zNear(zNear) {
    float t = std::tan(fov / 2.f) * 2;
    float a = zNear * t, b = a * aspectRatio;
    _vDir = glm::vec3(0, 1, 0) * a;
    _uDir = glm::vec3(1, 0, 0) * b;
    _lookAt = glm::vec3(0, 0, -1);
}

Camera::~Camera() {
}

Ray Camera::getRay(float u, float v) const {
    auto p = _uDir * u + _vDir * v + _lookAt * _zNear;
    return Ray(_pos, glm::normalize(p));
}
