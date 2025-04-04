#pragma once

#include "Material.h"
#include "../../OpenGLComponents/GLTexture2D.h"
#include <map>

class Material_Textures : public Material {
private:
	std::vector<GLTexture2D*> textures;
	std::vector<std::string> textureNames;

	std::map<std::string, float> floatUniforms;
	std::map<std::string, glm::vec2> vec2Uniforms;
	std::map<std::string, glm::vec3> vec3Uniforms;

	glm::vec2 textureScaling = glm::vec2(1.0);

public:
	Material_Textures();
	Material_Textures(ShaderProgramm shader);
	Material_Textures(const char* vertexShader, const char* fragmentShader);
	virtual ~Material_Textures();

	void Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model) override;

	//void AddTexture(std::string name, GLTexture2D texture);
	void AddTexture(std::string name, std::string texturePath);
	void SetFloatUniform(std::string name, float value);
	void SetVec2Uniform(std::string name, glm::vec2 value);
	void SetVec3Uniform(std::string name, glm::vec3 value);

	void RemoveTexture(std::string name);

private:
	void BindTextures();
	void BindFloats();
	void BindVec2s();
	void BindVec3s();
};