#include "ObjLoader.h"
char ObjLoader::lineBuffer[MAX_BUFFER];

void ObjLoader::load(const std::string& path) {
    Vertices.clear();
    Triangles.clear();
    FILE* file = fopen(path.c_str(), "r");
    if (!file) {
        std::cerr << "Cannot open file " << path << std::endl;
        return;
    }
    while (fgets(lineBuffer, MAX_BUFFER, file)) {
        _ptr = 0;
        process();
    }
}

void ObjLoader::process() {
    static char start[16];
    static char faceV[105];
    _ptr += sscanf(lineBuffer + _ptr, "%s", start);

    if (!strcmp(start, "v")) {
        double x, y, z;
        _ptr += sscanf(lineBuffer + _ptr, "%lf%lf%lf", &x, &y, &z);
        Vertices.push_back(glm::vec3(x, y, z));
    }
    else if (!strcmp(start, "f")) {
        int c = 0;
        std::vector<int> vertices;
        while (~(c = sscanf(lineBuffer + _ptr, "%s", faceV))) {
            int i = 0;
            int num = 0;
            bool neg = false;
            for (; faceV[i] != '\0' && faceV[i] != '/'; i++) {
                if (!isdigit(faceV[i])) {
                    if (faceV[i] == '-') {
                        neg = true;
                    }
                    continue;
                }
                // assert(isdigit(faceV[i]));
                num *= 10;
                num += faceV[i] - '0';
            }
            if (!neg)
                num--;
            else
                num = Vertices.size() - num - 1;
            vertices.push_back(num);
            _ptr += strlen(faceV) + c;
        }

        // Triangulation process
        auto cmp = [&](int a, int b) {
            return Vertices[a].x < Vertices[b].x;
        };
        std::sort(vertices.begin(), vertices.end());
        int sz = vertices.size();
        if (sz == 3) {
            Triangles.push_back(Face(vertices[0], vertices[1], vertices[2]));
        }
        else if (sz == 4) {
            glm::vec3 AB = Vertices[vertices[1]] - Vertices[vertices[0]];
            glm::vec3 C = glm::cross(AB, Vertices[vertices[2]] - Vertices[vertices[0]]);
            glm::vec3 D = glm::cross(AB, Vertices[vertices[3]] - Vertices[vertices[0]]);
            if (glm::dot(C, D) < 0) {
                std::swap(vertices[1], vertices[2]);
            }
            AB = Vertices[vertices[2]] - Vertices[vertices[1]];
            C = glm::cross(AB, Vertices[vertices[0]] - Vertices[vertices[1]]);
            D = glm::cross(AB, Vertices[vertices[3]] - Vertices[vertices[1]]);
            if (glm::dot(C, D) < 0) {
                std::swap(vertices[2], vertices[3]);
            }
            Triangles.push_back(Face(vertices[0], vertices[1], vertices[3]));
            Triangles.push_back(Face(vertices[3], vertices[1], vertices[2]));
        }
        else {
            assert(false);
        }
    }
}
