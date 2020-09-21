#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

#include "Scene.h"
#include "Structure/SceneObject/Sphere.h"
#include "Structure/SceneObject/Triangle.h"
#include "Structure/Utils.h"
#include "Structure/Materials/DiffuseMat.h"
#include "Structure/Materials/MetalMat.h"
#include "Structure/Materials/NormalMat.h"
#include "OBJ_Loader.hpp"

Scene::Scene() {
    std::shared_ptr<DiffuseMat> defMat = std::make_shared<DiffuseMat>();
    defMat->setDiffuseColor(glm::vec3(0.5));
    std::shared_ptr<MetalMat> metalMat = std::make_shared<MetalMat>(glm::vec3(0.8, 0.8, 0.8));
    std::shared_ptr<NormalMat> normalMat = std::make_shared<NormalMat>();
    auto sphere = std::make_shared<Sphere>(glm::vec3(3, 0, 0), 1, metalMat);
    auto sphere2 = std::make_shared<Sphere>(glm::vec3(2, 0, -2), 1, metalMat);
    auto tri1 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(5, -1, 5), glm::vec3(5, -1, -5), defMat);
    auto tri2 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(-5, -1, -5), glm::vec3(5, -1, -5), defMat);
    _sceneObjects.push_back(sphere);
    _sceneObjects.push_back(sphere2);
    _sceneObjects.push_back(tri1);
    _sceneObjects.push_back(tri2);

    objl::Loader loader;
    loader.LoadFile("models/spot.obj");
    for (auto mesh : loader.LoadedMeshes) {
        int sz = mesh.Indices.size();
        for (int i = 0; i < sz; i += 3) {
            std::vector<glm::vec3> vs;
            for (int j = 0; j < 3; j++) {
                auto& p = mesh.Vertices[mesh.Indices[i + j]].Position;
                vs.push_back(glm::vec3(p.X, p.Y, p.Z));
            }
            _sceneObjects.push_back(std::make_shared<Triangle>(vs, normalMat));
        }
    }
    printf("%d\n", _sceneObjects.size());

    //auto matLight = std::make_shared<Material>();
    //matLight->setReflectivity(0.0f);
    //matLight->setLightColor(glm::vec3(1, 1, 1));


    //auto matLight2 = std::make_shared<Material>();
    //matLight2->setReflectivity(0.0f);
    //matLight2->setLightColor(glm::vec3(0.8, 0.8, 0.8));

    //auto matNoLight = std::make_shared<Material>();
    //matNoLight->setLight(false);
    //matNoLight->setReflectivity(0.1f);
    //matNoLight->setDiffuseColor(glm::vec3(1, 1, 1));

    //auto matNoLight2 = std::make_shared<Material>();
    //matNoLight2->setLight(false);
    //matNoLight2->setReflectivity(1.f);
    //matNoLight2->setDiffuseColor(glm::vec3(1, 0, 0));
    ////_sceneObjects.push_back(std::make_shared<Circle>(glm::vec2(400, 150), 40, matNoLight));
    //_sceneObjects.push_back(std::make_shared<Circle>(glm::vec2(300, 400), 40, matLight));
    ////_sceneObjects.push_back(std::make_shared<Circle>(glm::vec2(500, 400), 40, matNoLight));
    //_sceneObjects.push_back(ShapeFactory::createRectangle(100, 100, glm::vec2(350, 250), matLight2));

    //_sceneObjects.push_back(ShapeFactory::createRectangle(700, 2, glm::vec2(50, 50), matNoLight));
    //_sceneObjects.push_back(ShapeFactory::createRectangle(700, 2, glm::vec2(50, 550), matNoLight));
    //_sceneObjects.push_back(ShapeFactory::createRectangle(2, 500, glm::vec2(50, 50), matNoLight));
    //_sceneObjects.push_back(ShapeFactory::createRectangle(2, 500, glm::vec2(750, 50), matNoLight));

    //_sceneObjects.push_back(ShapeFactory::createRectangle(2, 100, glm::vec2(150, 300), matNoLight2));
    //_sceneObjects.push_back(ShapeFactory::createRectangle(2, 100, glm::vec2(150, 150), matNoLight2));


    //glm::vec2 center(300, 200);
    //for (float r = 0; r > -glm::pi<float>() + 0.3f; r -= 0.05) {
    //    _sceneObjects.push_back(ShapeFactory::createSegment(center + glm::vec2(cos(r), sin(r)) * 50.f, center + glm::vec2(cos(r - 0.05f), sin(r - 0.05f)) * 50.f,
    //        matNoLight2));
    //}

    //_sceneObjects.push_back(std::make_shared<Circle>(glm::vec2(550, 300), 50, matNoLight);
    //float t;
    //bool b = raySegmentIntersect(Ray(glm::vec2(0, 0), glm::vec2(1, 0)), glm::vec2(0.5, 0), glm::vec2(1, 0), t);

    _bvhTree = std::make_unique<BVHTree>();
    _bvhTree->build(_sceneObjects);
}

Scene::~Scene() {
}

float clamp(float v, float minn, float maxx) {
    return std::min(std::max(v, minn), maxx);
}


bool Scene::castRay(const Ray& ray, IntersectionInfo& info) {
#ifdef BRUTE
    intersectInfo.setDistance(std::numeric_limits<float>::max());
    for (auto objs : _sceneObjects) {
        IntersectionInfo info;
        if (objs->rayInside(ray)) {
            return glm::vec3(1);
        }
        if (objs->rayIntersect(ray, info)) {
            if (info.getDistance() < intersectInfo.getDistance()) {
                intersectInfo = info;
            }
        }
    }
    return intersectInfo.getHitObject();
#else
    return _bvhTree->rayIntersect(ray, info);
#endif

}

void Scene::showDebugInfo(FrameBuffer& frame) {
    // _bvhTree->walkRectangles(frame);
}
