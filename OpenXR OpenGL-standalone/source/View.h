#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/GBuffer.h"

#include "OpenGLComponents/ShaderProgramm.h"

class Scene;

struct RenderInfo {
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::vec3 CameraPosition;
};

class View {
private:
    std::unique_ptr<GBuffer> gBuffer;
    std::unique_ptr<Framebuffer> framebuffer;

    std::unique_ptr<ShaderProgramm> lightingShader;
public:
    View(int width, int height);
    void DrawToGBuffer(Scene* scene, float deltaTime, RenderInfo& renderInfo);
    void CombineGBuffer(Scene* scene, RenderInfo& renderInfo);
    GLTexture2D* GetFramebufferTexture();

private:
    void BindLightsToLightingShader(Scene* scene);
};