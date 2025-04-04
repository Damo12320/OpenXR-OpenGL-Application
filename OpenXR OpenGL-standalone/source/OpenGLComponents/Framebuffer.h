#pragma once

#include "GLTexture2D.h"
#include <map>

class Framebuffer {
private:
	unsigned int ID;

	std::map<GLenum, std::unique_ptr<GLTexture2D>> attachedTextures;
public:
	Framebuffer();

	~Framebuffer();

	void AttachNewTexture2D(GLenum attachment, int widht, int height, int sizedInternalFormat = GL_RGBA8, int filter = GL_NEAREST, int mipMapLevels = -1);

	GLTexture2D* GetAttachedTexture(GLenum attachment);

	void Bind();

	unsigned int GetID();

	bool IsComplete();
};