#pragma once

#include "../OpenGLComponents/Framebuffer.h"
#include <memory>

class GBuffer {
private:
	std::unique_ptr<Framebuffer> framebuffer;
public:
	GBuffer(float width, float height);
	~GBuffer();

	void Bind();
	void Clear();

	GLTexture2D* GetPositionTexture();
	GLTexture2D* GetNormalTexture();
	GLTexture2D* GetAlbedoSpecTexture();
	GLTexture2D* GetDepthTexture();

private:
	void CreatePositionTexture(float width, float height);
	void CreateNormalTexture(float width, float height);
	void CreateAlbedoSpecTexture(float width, float height);
	void CreateDepthTexture(float width, float height);
};