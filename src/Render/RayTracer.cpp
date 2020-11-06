#include "RayTracer.h"
#include <stb_image.h>
#include <stb_image_write.h>
#include <glm/ext/matrix_transform.hpp>
#include <thread>
#include "Structure/Utils.h"
#include <ctime>
#include <mutex>
#include "Structure/SceneObject/Triangle.h"
#include "Structure/SceneObject/Sphere.h"
#include "Structure/Utils.h"

RayTracer::RayTracer(int width, int height) :_width(width), _height(height) {
    memset(stats, 0, sizeof(stats));
    _scene = std::make_unique<Scene>("scenes/gd1M3.sc");
    //_camera = std::make_unique<Camera>(glm::vec3(100, 200, 650), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::half_pi<float>(), 4.0f / 3.0f, 1);
    // _camera = std::make_unique<Camera>(glm::vec3(-6, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::half_pi<float>() / 2.5f, 4.0f / 3.0f, 1);
    // _camera = std::make_unique<Camera>(glm::vec3(-5, 5, 7), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::half_pi<float>() / 2.5f, 4.0f / 3.0f, 1);
}

RayTracer::~RayTracer() {
}
std::mutex mutex;
void RayTracer::run() {
    clock_t startTime, endTime;
    startTime = clock();

    FrameBuffer buffer(_width, _height, 3);
    int val = 0;
#ifdef MULTI
    constexpr int NUM_THREADS = 5;
    std::shared_ptr<std::thread> threads[NUM_THREADS];
    for (int k = 0; k < NUM_THREADS; k++) {
        threads[k] = std::make_shared<std::thread>([=, &buffer, &val]() {
            for (int i = k; i < _height; i += NUM_THREADS) {
                for (int j = 0; j < _width; j++) {
                    renderPos(glm::ivec2(j, i), buffer);
                }
                mutex.lock();
                val++;
                printf("%.2lf%%\n", val / (float)_height * 100);
                mutex.unlock();
            }
            });
    }
    for (int k = 0; k < NUM_THREADS; k++) threads[k]->join();
#else
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
#ifdef STATS
            countPos(glm::ivec2(j, i), buffer);
#else
            renderPos(glm::ivec2(j, i), buffer);
#endif
        }
        val++;
        printf("%.2lf%%\n", val / (float)_height * 100);
    }
#endif

#ifdef STATS
    int maxx = 0;
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            maxx = std::max(maxx, stats[i][j]);
        }
    }
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            buffer.set_pixel(glm::ivec2(j, i), glm::vec3(stats[i][j] / (float)(maxx)));
        }
    }
#endif
    _scene->showDebugInfo(buffer);
    //for (float r = 0; r < 6.28f; r += 0.5f) {
    //    Bresenham(glm::ivec2(100, 100), glm::ivec2(100 + cos(r) * 50, 100 + sin(r) * 50), buffer);
    //}
    ReportRandom();
    stbi_write_png("test.png", _width, _height, 3, buffer.getData(), _width * 3);

    endTime = clock();  //计时结束
    printf("The running time is: %.4lfs\n",
        (double)(endTime - startTime) / CLOCKS_PER_SEC);
}


glm::vec3 RayTracer::castRay(const Ray& ray, int depth) {
    if (depth == 1) return glm::vec3(0);
    IntersectionInfo intersectInfo;
    if (_scene->castRay(ray, intersectInfo)) {
        auto hitObj = intersectInfo.getHitObject();
        if (hitObj) {
            glm::vec3 attenuation;
            Ray new_ray(glm::vec3(0), glm::vec3(0));
            glm::vec3 reflectColor(0);
            glm::vec3 selfColor = hitObj->getMaterial()->getColor(ray, intersectInfo);
            if (hitObj->getMaterial()->scatter(ray, intersectInfo, attenuation, new_ray)) {
                reflectColor = attenuation * (castRay(new_ray, depth + 1) + sampleLight(intersectInfo));
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
    return  glm::vec3(1) * (1 - t) + glm::vec3(0.5, 0.7, 1.0) * t;
}

int RayTracer::countRay(const Ray& ray, int depth) {
    if (depth == 1) return 0;
    IntersectionInfo intersectInfo;
    int cnt = 0;
    if ((cnt = _scene->countRay(ray, intersectInfo)) != -1) {
        auto hitObj = intersectInfo.getHitObject();
        if (hitObj) {
            glm::vec3 attenuation;
            Ray new_ray(glm::vec3(0), glm::vec3(0));
            glm::vec3 reflectColor(0);
            glm::vec3 selfColor = hitObj->getMaterial()->getColor(ray, intersectInfo);
            if (hitObj->getMaterial()->scatter(ray, intersectInfo, attenuation, new_ray)) {
                cnt += countRay(new_ray, depth + 1);
            }
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
    return  cnt;
}

glm::vec3 RayTracer::sampleLight(const IntersectionInfo& info) {
    return _scene->sampleLight(info);
}

void RayTracer::renderPos(glm::ivec2 pos, FrameBuffer& buffer) {
    float sampleCount = 1;
    glm::vec3 totColor(0);
    for (int i = 0; i < sampleCount; i++) {
        glm::vec2 p2(randFloat(), randFloat());
        if (i == 0)
            p2 = glm::vec2(0.5);
        float x = (p2.x + pos.x - _width / 2) / _width;
        float y = (p2.y + pos.y - _height / 2) / _height;
        auto ray = _scene->getRay(x, y);
        totColor += castRay(ray, 0);
    }
    buffer.set_pixel(pos, totColor / sampleCount);
}

void RayTracer::countPos(glm::ivec2 pos, FrameBuffer& buffer) {
    glm::vec2 p2(randFloat(), randFloat());
    p2 = glm::vec2(0.5);
    float x = (p2.x + pos.x - _width / 2) / _width;
    float y = (p2.y + pos.y - _height / 2) / _height;
    auto ray = _scene->getRay(x, y);
    int cnt = countRay(ray, 0);
    stats[pos.y][pos.x] = cnt;
}
