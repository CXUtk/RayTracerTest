#pragma once
#include <glm/glm.hpp>
class Ray {
public:
    Ray(glm::vec3 start, glm::vec3 dir);
    glm::vec3 getStart() const { return _start; }
    glm::vec3 getDir() const { return _dir; }
private:
    glm::vec3 _start, _dir;
};
