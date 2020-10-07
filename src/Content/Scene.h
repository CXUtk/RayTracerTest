#pragma once
#include <vector>
#include <memory>
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/AccelStructure/AccelStructure.h"
#include "Structure/IntersectionInfo.h"
class Scene {
public:
    Scene();
    ~Scene();

    bool castRay(const Ray& ray, IntersectionInfo& info);
    void showDebugInfo(FrameBuffer& frame);
    glm::vec3 sampleLight(const IntersectionInfo& info);

private:
    std::vector<std::shared_ptr<Object>> _sceneObjects;
    std::unique_ptr<AccelStructure> _accelTree;
    std::vector<std::shared_ptr<Object>> _sceneLights;
};
