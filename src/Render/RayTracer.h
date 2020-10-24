#pragma once
#include "FrameBuffer.h"
#include "Structure/Ray.h"
#include "Content/Scene.h"
#include "Camera.h"
#include <memory>
class RayTracer {
public:
    RayTracer(int width, int height);
    ~RayTracer();

    void run();

private:
    glm::vec3 castRay(const Ray& ray, int depth);
    int countRay(const Ray& ray, int depth);
    glm::vec3 sampleLight(const IntersectionInfo& info);
    void renderPos(glm::ivec2 pos, FrameBuffer& buffer);
    void countPos(glm::ivec2 pos, FrameBuffer& buffer);
    int _width, _height;

    int stats[1024][1024];

    std::unique_ptr<Scene> _scene;


};
