#include "MaterialFactory.h"

//private static
MaterialFactory* MaterialFactory::instance = nullptr;
//private static
MaterialFactory* MaterialFactory::GetInstance() {
	if (instance == nullptr) {
		instance = new MaterialFactory();
	}
	return instance;
}



MaterialFactory::MaterialFactory() {
	//Create Materials
	MaterialCreateInfo_Textures materialCreateInfo_Textures{
		{
			"assets/shader/GBuffer.vert",
			"assets/shader/GBuffer_Albedo+NormalMap.frag"
		}
	};
	AddMaterialCreateInfo_Textures("simpleTextured", materialCreateInfo_Textures);

	MaterialCreateInfo_SolidColor materialCreateInfo_SolidColor{
		{
			"assets/shader/GBuffer.vert",
			"assets/shader/GBuffer_Color.frag"
		},
		1,
		glm::vec3(1, 1, 1)
	};
	AddMaterialCreateInfo_SolidColor("simpleColor", materialCreateInfo_SolidColor);

	MaterialCreateInfo_Textures materialCreateInfo_mountainSkybox{
		{
			"assets/shader/GBuffer.vert",
			"assets/shader/GBuffer_Albedo+NormalMap.frag"
		},
		{
			{"albedo", "assets/textures/Skybox_Diff.png"},
			{"normalMap", "assets/textures/Skybox_Normal.png"}
		}
	};
	AddMaterialCreateInfo_Textures("mountainSkybox", materialCreateInfo_mountainSkybox);

	MaterialCreateInfo_Textures materialCreateInfo_grass{
		{
			"assets/shader/GBuffer.vert",
			"assets/shader/GBuffer_Albedo+NormalMap.frag"
		},
		{
			{"albedo", "assets/textures/Grass.png"},
			{"normalMap", "assets/textures/GrassHigherDetail_normals.png"}
		}
	};
	AddMaterialCreateInfo_Textures("grass", materialCreateInfo_grass);

	MaterialCreateInfo_Textures materialCreateInfo_Textures_LCD{
		{
			"assets/shader/GBuffer.vert",
			"assets/shader/GBuffer_LCD_Display.frag"
		}
	};
	AddMaterialCreateInfo_Textures("LCD", materialCreateInfo_Textures_LCD);
}



void MaterialFactory::AddMaterialCreateInfo(std::string name, MaterialCreateInfo materialCreateInfo) {
	if (MaterialNameExists(name)) {
		std::cout << "MATERIALFACTORY::MaterialCreateInfo with name " << name << " already exists" << std::endl;
		return;
	}

	materialCreateInfos[name] = materialCreateInfo;
}
void MaterialFactory::AddMaterialCreateInfo_Textures(std::string name, MaterialCreateInfo_Textures materialCreateInfo_Textures) {
	if (MaterialNameExists(name)) {
		std::cout << "MATERIALFACTORY::MaterialCreateInfo with name " << name << " already exists" << std::endl;
		return;
	}
	materialCreateInfos_Textures[name] = materialCreateInfo_Textures;
}
void MaterialFactory::AddMaterialCreateInfo_SolidColor(std::string name, MaterialCreateInfo_SolidColor materialCreateInfo_SolidColor) {
	if (MaterialNameExists(name)) {
		std::cout << "MATERIALFACTORY::MaterialCreateInfo with name " << name << " already exists" << std::endl;
		return;
	}
	materialCreateInfos_SolidColor[name] = materialCreateInfo_SolidColor;
}


Material* MaterialFactory::CreateMaterialInstance(std::string name) {
	if (materialCreateInfos.find(name) != materialCreateInfos.end()) {
		return CreateMaterial(materialCreateInfos[name]);
	}
	else if (materialCreateInfos_Textures.find(name) != materialCreateInfos_Textures.end()) {
		return CreateMaterial_Textures(materialCreateInfos_Textures[name]);
	}
	else if (materialCreateInfos_SolidColor.find(name) != materialCreateInfos_SolidColor.end()) {
		return CreateMaterial_SolidColor(materialCreateInfos_SolidColor[name]);
	}
	else {
		return nullptr;
	}
}


Material* MaterialFactory::CreateMaterial(MaterialCreateInfo createInfo) {
	auto material = std::make_unique<Material>(createInfo.vertexShaderPath.c_str(), createInfo.fragmentShaderPath.c_str());
	Material* materialPtr = material.get();

	materials.push_back(std::move(material));

	return materialPtr;
}
Material_Textures* MaterialFactory::CreateMaterial_Textures(MaterialCreateInfo_Textures createInfo) {
	auto material = std::make_unique<Material_Textures>(createInfo.materialCreateInfo.vertexShaderPath.c_str(), createInfo.materialCreateInfo.fragmentShaderPath.c_str());
	for (auto& texture : createInfo.textures) {
		material->AddTexture(texture.first, texture.second.c_str());
	}
	Material_Textures* materialPtr = material.get();

	materials.push_back(std::move(material));

	return materialPtr;
}
Material_SolidColor* MaterialFactory::CreateMaterial_SolidColor(MaterialCreateInfo_SolidColor createInfo) {
	auto material = std::make_unique<Material_SolidColor>(createInfo.materialCreateInfo.vertexShaderPath.c_str(), createInfo.materialCreateInfo.fragmentShaderPath.c_str());
	material->SetColor(createInfo.color);
	material->SetSpecularStrength(1);
	Material_SolidColor* materialPtr = material.get();

	materials.push_back(std::move(material));

	return materialPtr;
}

bool MaterialFactory::MaterialNameExists(std::string name) {
	return materialCreateInfos.find(name) != materialCreateInfos.end() 
		|| materialCreateInfos_Textures.find(name) != materialCreateInfos_Textures.end()
		|| materialCreateInfos_SolidColor.find(name) != materialCreateInfos_SolidColor.end();
}