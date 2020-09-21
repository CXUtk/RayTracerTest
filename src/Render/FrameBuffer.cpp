#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(int width, int height, int numChannel) : _numChannel(numChannel), _width(width), _height(height) {
    _data = new unsigned char[width * height * numChannel];
    memset(_data, 0, sizeof(unsigned char) * width * height * numChannel);
}

void FrameBuffer::set_pixel(glm::ivec2 pos, glm::vec3 color) {
    if (pos.x < 0 || pos.x >= _width || pos.y < 0 || pos.y >= _height) return;
    pos.y = _height - 1 - pos.y;
    int start = _numChannel * (pos.y * _width + pos.x);

    color.r = sqrt(color.r);
    color.g = sqrt(color.g);
    color.b = sqrt(color.b);

    _data[start] = (unsigned char)floor(glm::clamp(color.r, 0.f, 0.999f) * 256);
    _data[start + 1] = (unsigned char)floor(glm::clamp(color.g, 0.f, 0.999f) * 256);
    _data[start + 2] = (unsigned char)floor(glm::clamp(color.b, 0.f, 0.999f) * 256);
}
