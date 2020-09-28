#include "Utils.h"

static std::mt19937 random(114514);
static long long NumCalls = 0;

int dcmp(float v) {
    if (fabs(v) < EPS) return 0;
    return v > 0 ? 1 : -1;
}

glm::vec2 lineIntersection(glm::vec2 l1, glm::vec2 d1, glm::vec2 l2, glm::vec2 d2) {
    float t = cross2(d2, l1 - l2) / cross2(d1, d2);
    return l1 + d1 * t;
}


float randFloat() {
    NumCalls++;
    return random() / (float)random.max();
}

void ReportRandom() {
    printf("%lld\n", NumCalls);
}

glm::vec3 randVec3() {
    return glm::vec3(randFloat() * 2 - 1, randFloat() * 2 - 1, randFloat() * 2 - 1);
}

glm::vec3 randSphereVec() {
    float a = randFloat() * glm::two_pi<float>();
    float z = randFloat() * 2 - 1;
    float r = sqrt(1 - z * z);
    return glm::vec3(r * cos(a), r * sin(a), z);
}

void drawRectangle(glm::vec2 start, glm::vec2 size, FrameBuffer& frame) {
    glm::vec3 red = glm::vec3(1, 0, 0);
    for (int i = start.x; i <= start.x + size.x; i++) {
        frame.set_pixel(glm::ivec2(i, start.y), red);
        frame.set_pixel(glm::ivec2(i, start.y + size.y), red);
    }
    for (int i = start.y; i <= start.y + size.y; i++) {
        frame.set_pixel(glm::ivec2(start.x, i), red);
        frame.set_pixel(glm::ivec2(start.x + size.x, i), red);
    }
}

void Bresenham(glm::ivec2 start, glm::ivec2 end, FrameBuffer& frame) {
    bool swp = false;
    int dx = end.x - start.x, dy = end.y - start.y;
    int yInc = 1;
    if (abs(dx) < abs(dy)) {
        std::swap(dx, dy);
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
        swp = true;
    }
    if (dx < 0) {
        std::swap(start.x, end.x);
        std::swap(start.y, end.y);
        dx *= -1, dy *= -1;
    }
    if (dy < 0) {
        yInc = -1;
        dy *= -1;
    }
    int a = 2 * dy, b = 2 * (dy - dx);
    int cur = 2 * dy - dx;
    glm::vec3 red = glm::vec3(1, 0, 0);
    for (int i = start.x, j = start.y; i <= end.x; i++) {
        if (swp)
            frame.set_pixel(glm::ivec2(j, i), red);
        else
            frame.set_pixel(glm::ivec2(i, j), red);
        if (cur <= 0) {
            cur += a;
        }
        else {
            j += yInc;
            cur += b;
        }
    }
}

bool raySegmentIntersect(const Ray& ray, glm::vec2 C, glm::vec2 D, float& t) {
    glm::vec2 A = ray.getStart();
    glm::vec2 B = ray.getDir();
    float a = B.x, b = D.x - C.x;
    float c = B.y, d = D.y - C.y;
    float det = a * d - b * c;
    t = 0;
    if (!dcmp(det)) return false;
    float k1 = D.x - A.x;
    float k2 = D.y - A.y;
    t = (d * k1 - b * k2) / det;
    if (t < 0) return false;
    float s = (-c * k1 + a * k2) / det;
    if (s < 0 || s > 1) return false;
    return true;
}
