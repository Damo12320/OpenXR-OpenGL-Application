#pragma once

#include "Material.h"

class Material_SolidColor : public Material {
	glm::vec3 color = glm::vec3(1.0f);
	float specularStrength = 0.0f;
public:
	Material_SolidColor();
	Material_SolidColor(ShaderProgramm shader);
	Material_SolidColor(const char* vertexShader, const char* fragmentShader);
	virtual ~Material_SolidColor();

	void Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model) override;

	void SetSpecularStrength(float specularStrength);
	void SetColor(glm::vec3 color);
};