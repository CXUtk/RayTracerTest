#pragma once
#include <vector>
#include <memory>
#include "Structure/Object.h"
#include "Render/FrameBuffer.h"
#include "Structure/AccelStructure/AccelStructure.h"
#include "Structure/IntersectionInfo.h"
#include "Render/Camera.h"
class Scene {
public:
    Scene(const std::string& condigFile);
    ~Scene();

    Ray getRay(float x, float y);
    bool castRay(const Ray& ray, IntersectionInfo& info);
    int countRay(const Ray& ray, IntersectionInfo& info);
    void showDebugInfo(FrameBuffer& frame);
    glm::vec3 sampleLight(const IntersectionInfo& info);

private:
    std::unique_ptr<Camera> _camera;
    std::vector<std::shared_ptr<Object>> _sceneObjects;
    std::unique_ptr<AccelStructure> _accelTree;
    std::vector<std::shared_ptr<Object>> _sceneLights;

    void loadSceneObjects(std::ifstream& stream);
    void loadCamera(std::ifstream& stream);
};
