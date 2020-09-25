#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <Structure\Ray.h>
class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);
    ~BoundingBox();

    glm::vec3 getMinPos()const { return _minPos; }
    glm::vec3 getMaxPos()const { return _maxPos; }

    bool rayIntersect(const Ray& ray, float& tNear) const;

    glm::vec3 getCenter() const { return (_minPos + _maxPos) / 2.f; }

    BoundingBox Union(const BoundingBox& box) const {
        glm::vec3 minn(std::min(_minPos.x, box._minPos.x), std::min(_minPos.y, box._minPos.y), std::min(_minPos.z, box._minPos.z));
        glm::vec3 maxx(std::max(_maxPos.x, box._maxPos.x), std::max(_maxPos.y, box._maxPos.y), std::max(_maxPos.z, box._maxPos.z));
        return BoundingBox(minn, maxx);
    }
    BoundingBox Intersection(const BoundingBox& box) const {
        glm::vec3 minn(std::max(_minPos.x, box._minPos.x), std::max(_minPos.y, box._minPos.y), std::max(_minPos.z, box._minPos.z));
        glm::vec3 maxx(std::min(_maxPos.x, box._maxPos.x), std::min(_maxPos.y, box._maxPos.y), std::min(_maxPos.z, box._maxPos.z));
        return BoundingBox(minn, maxx);
    }

private:
    glm::vec3 _minPos, _maxPos;
};
