#pragma once

#include "Material.h"
#include "Material_Textures.h"
#include "Material_SolidColor.h"

#include <map>

struct MaterialCreateInfo {
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
};
struct MaterialCreateInfo_Textures {
	MaterialCreateInfo materialCreateInfo;
	std::vector<std::pair<std::string, std::string>> textures;
	std::vector<std::pair<std::string, float>> floatUniforms;
	std::vector<std::pair<std::string, glm::vec2>> vec2Uniforms;
	std::vector<std::pair<std::string, glm::vec3>> vec3Uniforms;
};
struct MaterialCreateInfo_SolidColor {
	MaterialCreateInfo materialCreateInfo;
	float specularStrength = 0.0f;
	glm::vec3 color;
};

class MaterialFactory {
private:
	static MaterialFactory* instance;
	MaterialFactory();

	std::map<std::string, MaterialCreateInfo> materialCreateInfos;
	std::map<std::string, MaterialCreateInfo_Textures> materialCreateInfos_Textures;
	std::map<std::string, MaterialCreateInfo_SolidColor> materialCreateInfos_SolidColor;

	std::vector<std::unique_ptr<Material>> materials;

public:
	static MaterialFactory* GetInstance();

	void AddMaterialCreateInfo(std::string name, MaterialCreateInfo materialCreateInfo);
	void AddMaterialCreateInfo_Textures(std::string name, MaterialCreateInfo_Textures materialCreateInfo_Textures);
	void AddMaterialCreateInfo_SolidColor(std::string name, MaterialCreateInfo_SolidColor materialCreateInfo_SolidColor);

	Material* CreateMaterialInstance(std::string name);

private:
	Material* CreateMaterial(MaterialCreateInfo createInfo);
	Material_Textures* CreateMaterial_Textures(MaterialCreateInfo_Textures createInfo);
	Material_SolidColor* CreateMaterial_SolidColor(MaterialCreateInfo_SolidColor createInfo);

	bool MaterialNameExists(std::string name);
};