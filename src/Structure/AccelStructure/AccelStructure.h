#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Structure/Object.h"
class AccelStructure {
public:
    AccelStructure() { _totWalks = _maxWalks = _totNums = _maxNums = _callCnt = 0; }
    virtual void build(const std::vector<std::shared_ptr<Object>>& objects) = 0;
    virtual bool rayIntersect(const Ray& ray, IntersectionInfo& info) const = 0;
    virtual int rayIntersectCount(const Ray& ray, IntersectionInfo& info) const = 0;
    virtual void report() const {
        printf("Total Called: %lld\n", _callCnt);
        printf("Total Step: %lld\n", _totWalks);
        printf("Max Step: %lld\n", _maxWalks);
        printf("Average Step: %lf\n", _totWalks / (double)_callCnt);
        printf("Total intersections: %lld\n", _totNums);
        printf("Max intersections: %lld\n", _maxNums);
        printf("Average intersections per ray: %lf\n", _totNums / (double)_callCnt);
    }
    virtual int numOfNodes() const { return 0; }

    mutable long long _totWalks, _maxWalks, _totNums, _maxNums, _callCnt;

    static std::unique_ptr<AccelStructure> makeAccelStructure(const std::string& name, int maxnodes);
};
