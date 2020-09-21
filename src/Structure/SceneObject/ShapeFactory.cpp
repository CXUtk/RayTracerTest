#include "ShapeFactory.h"

std::shared_ptr<Polygon> ShapeFactory::createRectangle(float width, float height, glm::vec2 pos, std::shared_ptr<Material> material) {
    static std::vector<glm::vec2> rect{ glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) };
    glm::mat3 transform = glm::identity<glm::mat3>();
    transform = glm::translate(transform, pos);
    transform = glm::scale(transform, glm::vec2(width, height));
    return std::make_shared<Polygon>(rect, material, transform);
}

std::shared_ptr<Segment> ShapeFactory::createSegment(glm::vec2 start, glm::vec2 end, std::shared_ptr<Material> material) {
    auto p = std::make_shared<Segment>(start, end);
    p->setMaterial(material);
    return p;
}
