#include <Render/Renderer.h>

int main() {
    auto& renderer = Renderer::GetInstance();
    renderer.run();
    return 0;
}
