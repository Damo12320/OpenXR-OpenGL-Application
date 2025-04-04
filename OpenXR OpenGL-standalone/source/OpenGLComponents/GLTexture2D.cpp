#include "GLTexture2D.h"

GLTexture2D::GLTexture2D(const char* texturePath) {
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::Failed to load texture" << std::endl;
	}

	this->width = width;
	this->height = height;
	stbi_image_free(data);
}

GLTexture2D::GLTexture2D(int width, int height, int sizedInternalFormat, int mipMapLevels) {
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	if (mipMapLevels == -1)
	{
		/*int max = std::max(width, height);
		mipMapLevels = std::log2f(std::floor(std::max(1, max))) + 1;*/
		mipMapLevels = std::log2f(std::floor(std::max(1, std::max(width, height)))) + 1;
	}

	glTextureStorage2D(this->ID, mipMapLevels, sizedInternalFormat, width, height);

	this->width = width;
	this->height = height;
}





GLTexture2D::~GLTexture2D() {
	std::cout << "TEXTURE2D::Deleting" << std::endl;
	glDeleteTextures(1, &this->ID);
}

void GLTexture2D::BindToUnit(GLuint unit) {
	glBindTextureUnit(unit, this->ID);
}

//private static
std::map<std::string, std::unique_ptr<GLTexture2D>> GLTexture2D::loadedTextures = {};
//public static
GLTexture2D* GLTexture2D::LoadTextureFromPath(std::string path) {
	if (loadedTextures.find(path) != loadedTextures.end()) {
		return loadedTextures[path].get();
	}

	std::unique_ptr<GLTexture2D> texture = std::make_unique<GLTexture2D>(path.c_str());
	GLTexture2D* tex = texture.get();
	loadedTextures[path] = std::move(texture);

	return tex;
}