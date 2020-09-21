#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Renderer {
public:
    static Renderer& GetInstance();
    ~Renderer();
    void run();


    void setWidth(int value) { _width = value; }
    void setHeight(int value) { _height = value; }

private:
    Renderer(int width, int height);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void recalculateProjectionMatrix();
    void generatePNG();


    int _width, _height;
    GLFWwindow* _window;
    glm::mat4 _projectionMatrix;

};
