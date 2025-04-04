#pragma once

#include <glad/glad.h>
#include "../imported/stb_image.h"
#include <iostream>
#include <cmath>
#include <map>

class GLTexture2D {
private:
	static std::map<std::string, std::unique_ptr<GLTexture2D>> loadedTextures;

	unsigned int ID;

	int width;
	int height;
public://Getters and Setters
	unsigned int GetID() { return this->ID; }
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }
public:
	GLTexture2D(const char* texturePath);
	GLTexture2D(int width, int height, int sizedInternalFormat = GL_RGBA8, int mipMapLevels = -1);

	~GLTexture2D();

	void BindToUnit(GLuint unit);

	static GLTexture2D* LoadTextureFromPath(std::string path);
};