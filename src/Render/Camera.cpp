#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookat, const glm::vec3& up, float fov, float aspectRatio, float zNear) :_pos(pos), _fov(fov), _aspectRatio(aspectRatio), _zNear(zNear) {
    // Gram-schmidt process to get a orthogonal basis
    float t = std::tan(fov / 2.f) * 2;
    float a = zNear * t, b = a * aspectRatio;
    auto look = glm::normalize(lookat - pos);
    auto p = glm::dot(up, look) * look;
    _vDir = glm::normalize(up - p);
    _uDir = glm::cross(look, _vDir);
    _vDir *= a, _uDir *= b;
    _lookAt = look;
}

Camera::~Camera() {
}

Ray Camera::getRay(float u, float v) const {
    auto p = _uDir * u + _vDir * v + _lookAt * _zNear;
    return Ray(_pos, glm::normalize(p));
}
