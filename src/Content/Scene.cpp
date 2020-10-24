#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "Scene.h"
#include "Structure/SceneObject/Sphere.h"
#include "Structure/SceneObject/Triangle.h"
#include "Structure/Utils.h"
#include "Structure/Materials/DiffuseMat.h"
#include "Structure/Materials/MetalMat.h"
#include "Structure/Materials/NormalMat.h"
#include "Structure/Materials/GlassMat.h"
#include "Structure/Materials/LightMat.h"
#include "Structure/SceneObject/TriangleMesh.h"

#include "OBJ_Loader.hpp"

Scene::Scene(const std::string& configFile) {
    std::ifstream input(configFile, std::ios_base::in);
    loadCamera(input);

    loadSceneObjects(input);

    //std::shared_ptr<DiffuseMat> defMat = std::make_shared<DiffuseMat>();
    //defMat->setDiffuseColor(glm::vec3(0.5));

    //std::shared_ptr<DiffuseMat> redMat = std::make_shared<DiffuseMat>();
    //std::shared_ptr<GlassMat> glassMat = std::make_shared<GlassMat>();
    //redMat->setDiffuseColor(glm::vec3(0.7, 0.2, 0.2));
    //std::shared_ptr<MetalMat> metalMat = std::make_shared<MetalMat>(glm::vec3(0.9, 0.9, 0.7), 0.15f);
    //std::shared_ptr<LightMat> lightMat = std::make_shared<LightMat>(glm::vec3(3));

    //auto sphere = std::make_shared<Sphere>(glm::vec3(3, 10, -3), 1, lightMat);//std::make_shared<Sphere>(glm::vec3(0, 5, 0), 1, lightMat);
    //auto sphere2 = std::make_shared<Sphere>(glm::vec3(0, 0, -3), 1, redMat);
    //auto tri1 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(5, -1, 5), glm::vec3(5, -1, -5), defMat);
    //auto tri2 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(-5, -1, -5), glm::vec3(5, -1, -5), defMat);
    //_sceneLights.push_back(sphere);
    //_sceneObjects.push_back(sphere);
    //_sceneObjects.push_back(sphere2);
    //_sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(-2, 0, 2), 0.5, glassMat));
    //_sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(2, 0, 2), 0.5, metalMat));
    //// _sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(0, -0.5, 3), 0.5, glassMat));
    //_sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(0, -0.5, 3), 0.5, std::make_shared<DiffuseMat>(glm::vec3(0.5, 1, 0.5))));
    //_sceneObjects.push_back(tri1);
    //_sceneObjects.push_back(tri2);


    printf("%d\n", _sceneObjects.size());
    _accelTree = AccelStructure::makeAccelStructure("KD", 1048576);
    _accelTree->build(_sceneObjects);
    printf("%d\n", _accelTree->numOfNodes());
    printf("Build Complete\n");
}

Scene::~Scene() {
}

float clamp(float v, float minn, float maxx) {
    return std::min(std::max(v, minn), maxx);
}

void Scene::loadSceneObjects(std::ifstream& input) {
    int n;
    input >> n;
    std::shared_ptr<DiffuseMat> defMat = std::make_shared<DiffuseMat>();
    defMat->setDiffuseColor(glm::vec3(0.5));
    std::shared_ptr<DiffuseMat> redMat = std::make_shared<DiffuseMat>();
    std::shared_ptr<GlassMat> glassMat = std::make_shared<GlassMat>();
    redMat->setDiffuseColor(glm::vec3(0.7, 0.2, 0.2));
    std::shared_ptr<MetalMat> metalMat = std::make_shared<MetalMat>(glm::vec3(0.9, 0.9, 0.7), 0.15f);
    std::shared_ptr<LightMat> lightMat = std::make_shared<LightMat>(glm::vec3(3));

    for (int i = 0; i < n; i++) {
        std::string filename;
        input >> filename;

        objl::Loader loader;
        loader.LoadFile(filename);
        glm::vec3 trans, scale;
        input >> scale.x >> scale.y >> scale.z;
        input >> trans.x >> trans.y >> trans.z;


        glm::mat4 transform = glm::identity<glm::mat4>();
        transform = glm::translate(transform, trans);
        transform = glm::scale(transform, scale);


        for (auto& mesh : loader.LoadedMeshes) {
            int sz = mesh.Indices.size();
            for (int i = 0; i < sz; i += 3) {
                std::vector<glm::vec3> vs;
                for (int j = 0; j < 3; j++) {
                    auto& p = mesh.Vertices[mesh.Indices[i + j]].Position;
                    glm::vec4 x(p.X, p.Y, p.Z, 1);
                    x = transform * x;
                    vs.push_back(x);
                }
                _sceneObjects.push_back(std::make_shared<Triangle>(vs, defMat));
            }
            //_sceneObjects.push_back(std::make_shared<TriangleMesh>(trimesh));
            //trimesh.clear();
        }

    }


    auto sphere = std::make_shared<Sphere>(glm::vec3(3, 10, -3), 1, lightMat);//std::make_shared<Sphere>(glm::vec3(0, 5, 0), 1, lightMat);
    auto sphere2 = std::make_shared<Sphere>(glm::vec3(0, 0, -3), 1, redMat);
    auto tri1 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(5, -1, 5), glm::vec3(5, -1, -5), defMat);
    auto tri2 = std::make_shared<Triangle>(glm::vec3(-5, -1, 5), glm::vec3(-5, -1, -5), glm::vec3(5, -1, -5), defMat);
    _sceneLights.push_back(sphere);
    _sceneObjects.push_back(sphere);
    _sceneObjects.push_back(sphere2);
    _sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(-2, 0, 2), 0.5, glassMat));
    _sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(2, 0, 2), 0.5, metalMat));
    // _sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(0, -0.5, 3), 0.5, glassMat));
    _sceneObjects.push_back(std::make_shared<Sphere>(glm::vec3(0, -0.5, 3), 0.5, std::make_shared<DiffuseMat>(glm::vec3(0.5, 1, 0.5))));
    _sceneObjects.push_back(tri1);
    _sceneObjects.push_back(tri2);

    //objl::Loader loader;
    //loader.LoadFile("models/spot.obj");
    //std::map<std::string, std::shared_ptr<Material>> matTable;
    //for (auto& mat : loader.LoadedMaterials) {
    //    std::shared_ptr<Material> matR = std::make_shared<DiffuseMat>(glm::vec3(mat.Kd.X, mat.Kd.Y, mat.Kd.Z));
    //    matTable[mat.name] = matR;
    //}

    //glm::mat4 transform = glm::identity<glm::mat4>();
    ////transform = glm::translate(transform, glm::vec3(0, -1, 0));
    ////transform = glm::scale(transform, glm::vec3(15, 15, 15));
    //transform = glm::translate(transform, glm::vec3(0, 0, 0));
    //transform = glm::scale(transform, glm::vec3(1, 1, 1));
    //std::vector<std::shared_ptr<Triangle>> trimesh;
    //for (auto& mesh : loader.LoadedMeshes) {
    //    int sz = mesh.Indices.size();
    //    std::shared_ptr<Material> mat = defMat;

    //    if (mesh.MeshMaterial.has_value() && matTable.find(mesh.MeshMaterial->name) != matTable.end()) {
    //        mat = matTable[mesh.MeshMaterial->name];
    //        if (mesh.MeshMaterial->name == "glass") {
    //            mat = glassMat;
    //        }
    //    }
    //    for (int i = 0; i < sz; i += 3) {
    //        std::vector<glm::vec3> vs;
    //        for (int j = 0; j < 3; j++) {
    //            auto& p = mesh.Vertices[mesh.Indices[i + j]].Position;
    //            glm::vec4 x(p.X, p.Y, p.Z, 1);
    //            x = transform * x;
    //            vs.push_back(x);
    //        }
    //        _sceneObjects.push_back(std::make_shared<Triangle>(vs, mat));
    //    }
    //    //_sceneObjects.push_back(std::make_shared<TriangleMesh>(trimesh));
    //    //trimesh.clear();
    //        }
}

void Scene::loadCamera(std::ifstream& stream) {
    glm::vec3 campos, lookat, up;
    stream >> campos.x >> campos.y >> campos.z;
    stream >> lookat.x >> lookat.y >> lookat.z;
    stream >> up.x >> up.y >> up.z;

    float fov, aspect;
    stream >> fov >> aspect;

    _camera = std::make_unique<Camera>(campos, lookat, up, fov, aspect, 1);
}


Ray Scene::getRay(float x, float y) {
    return _camera->getRay(x, y);
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
    return _accelTree->rayIntersect(ray, info);
#endif

}

int Scene::countRay(const Ray& ray, IntersectionInfo& info) {
    return _accelTree->rayIntersectCount(ray, info);
}

void Scene::showDebugInfo(FrameBuffer& frame) {
    // _accelTree->walkRectangles(frame);
    _accelTree->report();
}

glm::vec3 Scene::sampleLight(const IntersectionInfo& info) {
    auto normal = info.getNormal();
    glm::vec3 color(0);
    for (auto light : _sceneLights) {
        if (randFloat() > 1.f / light->emitArea()) continue;
        auto pos = light->uniformGetSurface();
        auto dir = pos - info.getHitPos();
        auto c = glm::clamp(glm::dot(normal, dir) / glm::dot(dir, dir) * light->emitArea(), 0.f, 1.f);

        dir = glm::normalize(dir);
        IntersectionInfo tmp;
        auto ray = Ray(info.getHitPos() + dir * 0.0001f, dir);
        if (castRay(ray, tmp) && tmp.getHitObject() == light.get())
            color += c * light->getMaterial()->getColor(ray, tmp);
    }
    return color;
}
