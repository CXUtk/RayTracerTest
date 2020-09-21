#pragma once
#include <glm/glm.hpp>
class FrameBuffer {
public:
    FrameBuffer(int width, int height, int numChannel);

    void set_pixel(glm::ivec2 pos, glm::vec3 color);
    const unsigned char* getData()const { return _data; }

private:
    unsigned char* _data;
    int _numChannel;
    int _width, _height;
};
