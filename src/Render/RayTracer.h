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
    void renderPos(glm::ivec2 pos, FrameBuffer& buffer);
    int _width, _height;

    std::unique_ptr<Camera> _camera;
    std::unique_ptr<Scene> _scene;

};
