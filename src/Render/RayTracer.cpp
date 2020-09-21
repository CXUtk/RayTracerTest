#include "RayTracer.h"
#include <stb_image.h>
#include <stb_image_write.h>
#include <glm/ext/matrix_transform.hpp>
#include <thread>
#include "Structure/Utils.h"
#include <ctime>
#include "Structure/SceneObject/Triangle.h"
#include "Structure/SceneObject/Sphere.h"

RayTracer::RayTracer(int width, int height) :_width(width), _height(height) {
    _scene = std::make_unique<Scene>();
    _camera = std::make_unique<Camera>(glm::vec3(0, 1.2, 5), glm::half_pi<float>(), 4.0f / 3.0f, 1);
}

RayTracer::~RayTracer() {
}

void RayTracer::run() {
    clock_t startTime, endTime;
    startTime = clock();

    FrameBuffer buffer(_width, _height, 3);
    for (int i = 0; i < _height; i += 10) {
        std::shared_ptr<std::thread> threads[10];
        for (int k = 0; k < 10; k++) {
            threads[k] = std::make_shared<std::thread>([=, &buffer]() {
                for (int j = 0; j < _width; j++) {
                    renderPos(glm::ivec2(j, i + k), buffer);
                }
                }
            );
        }
        for (int k = 0; k < 10; k++) threads[k]->join();
        printf("%.2lf%%\n", i / (float)_height);
    }
    //_scene->showDebugInfo(buffer);
    //for (float r = 0; r < 6.28f; r += 0.5f) {
    //    Bresenham(glm::ivec2(100, 100), glm::ivec2(100 + cos(r) * 50, 100 + sin(r) * 50), buffer);
    //}
    stbi_write_png("test.png", _width, _height, 3, buffer.getData(), _width * 3);

    endTime = clock();  //计时结束
    printf("The running time is: %.4lfs\n",
        (double)(endTime - startTime) / CLOCKS_PER_SEC);
}


glm::vec3 RayTracer::castRay(const Ray& ray, int depth) {
    if (depth == 7) return glm::vec3(0);
    IntersectionInfo intersectInfo;
    if (_scene->castRay(ray, intersectInfo)) {
        auto hitObj = intersectInfo.getHitObject();
        if (hitObj) {
            glm::vec3 attenuation;
            Ray new_ray(glm::vec3(0), glm::vec3(0));
            glm::vec3 reflectColor(0);
            glm::vec3 selfColor = hitObj->getMaterial()->getColor(ray, intersectInfo);
            if (hitObj->getMaterial()->scatter(ray, intersectInfo, attenuation, new_ray)) {
                reflectColor = attenuation * castRay(new_ray, depth + 1);
            }
            return selfColor + reflectColor;

            //auto normal = intersectInfo.getNormal();
            //float r = hitObj->getMaterial()->getReflectivity();
            //auto hitPos = intersectInfo.getHitPos();
            //glm::vec2 p = hitPos + normal;
            //float rad = randFloat() * glm::pi<float>() - glm::pi<float>() / 2;
            //float rad2 = atan2(normal.y, normal.x) + rad;
            ////glm::vec2 dir2 = glm::vec2(cos(rad2), sin(rad2));
            //glm::vec2 dir2 = glm::reflect(ray.getDir(), normal);
            //glm::vec3 diffuse = hitObj->getDiffuseColor(glm::vec2(0));
            //float d = intersectInfo.getDistance() / 200.f;
            //if (hitObj->getMaterial()->isLight()) {
            //    return (hitObj->getSampleColor(glm::vec2(0)) + r * diffuse * castRay(Ray(hitPos + normal, dir2), depth + 1) * glm::pi<float>());
            //}
            //return r * diffuse * castRay(Ray(hitPos + normal, dir2), depth + 1) * glm::pi<float>();
        }
    }
    float t = 0.5f * (ray.getDir().y + 1);
    return glm::vec3(1) * (1 - t) + glm::vec3(0.5, 0.7, 1.0) * t;
}

void RayTracer::renderPos(glm::ivec2 pos, FrameBuffer& buffer) {
    float sampleCount = 64;
    glm::vec3 totColor(0);
    for (int i = 0; i < sampleCount; i++) {
        glm::vec2 p2(randFloat(), randFloat());
        if (i == 0)
            p2 = glm::vec2(0.5);
        float x = (p2.x + pos.x - _width / 2) / _width;
        float y = (p2.y + pos.y - _height / 2) / _height;
        auto ray = _camera->getRay(x, y);
        totColor += castRay(ray, 0);
    }
    buffer.set_pixel(pos, totColor / sampleCount);
}
