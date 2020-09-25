#pragma once
#include <glm/glm.hpp>
#include "Structure/Ray.h"
class Camera {
public:
    Camera(const glm::vec3& pos, const glm::vec3& lookat, const glm::vec3& up, float fov, float aspectRatio, float zNear);
    ~Camera();
    Ray getRay(float u, float v) const;

private:
    glm::vec3 _pos;
    float _fov, _aspectRatio, _zNear;
    glm::vec3 _uDir, _vDir, _lookAt;
};
