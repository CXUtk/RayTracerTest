#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "RayTracer.h"

Renderer& Renderer::GetInstance() {
    static Renderer renderer(800, 600);
    return renderer;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer& renderer = Renderer::GetInstance();
    renderer.setWidth(width);
    renderer.setHeight(height);
}

Renderer::Renderer(int width, int height) : _width(width), _height(height) {

    // Initialize OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(width, height, "2D Ray Tracer", nullptr, nullptr);
    if (!_window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load glad!" << std::endl;
        glfwTerminate();
        return;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    recalculateProjectionMatrix();

    generatePNG();
}

void Renderer::recalculateProjectionMatrix() {
    _projectionMatrix = glm::ortho(0.f, static_cast<float>(_width), 0.f, static_cast<float>(_height));
}

void Renderer::generatePNG() {
    RayTracer tracer(_width, _height);
    tracer.run();
}

Renderer::~Renderer() {
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Renderer::run() {

    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
