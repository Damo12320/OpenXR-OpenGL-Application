#include "View.h"
#include "Engine/Scene.h"

View::View(int width, int height) {
	// Create GBuffer
	this->gBuffer = std::make_unique<GBuffer>(width, height);

	// Create Framebuffer
	this->framebuffer = std::make_unique<Framebuffer>();
	this->framebuffer->AttachNewTexture2D(GL_COLOR_ATTACHMENT0, width, height, GL_RGBA32F);

	if (!this->framebuffer->IsComplete()) {
		std::cout << "VIEW::Framebuffer not complete" << std::endl;
	}

	// Create Lighting Shader
	const char* vertexPath = "assets/shader/LightingPass.vert";
	const char* fragmentPath = "assets/shader/LightingPass.frag";
	this->lightingShader = std::make_unique<ShaderProgramm>(vertexPath, fragmentPath);
}

void View::DrawToGBuffer(Scene* scene, float deltaTime, RenderInfo& renderInfo) {
	gBuffer->Bind();
	gBuffer->Clear();
	
	glEnable(GL_DEPTH_TEST);

	scene->Draw(renderInfo);
}

void View::CombineGBuffer(Scene* scene, RenderInfo& renderInfo) {
	this->framebuffer->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->lightingShader->Bind();

	//Bind GBuffer Textures
	this->lightingShader->SetInt("gPosition", 0);
	this->lightingShader->SetInt("gNormal", 1);
	this->lightingShader->SetInt("gAlbedoSpec", 2);

	this->gBuffer->GetPositionTexture()->BindToUnit(0);
	this->gBuffer->GetNormalTexture()->BindToUnit(1);
	this->gBuffer->GetAlbedoSpecTexture()->BindToUnit(2);

	//Bind Lights
	this->BindLightsToLightingShader(scene);

	//Bind Ambient Light
	glm::vec3 ambientLightColor = glm::vec3(0.1f, 0.1f, 0.1f);
	this->lightingShader->SetVec3("ambientLightColor", ambientLightColor);

	//Bind Camera Position
	this->lightingShader->SetVec3("viewPos", renderInfo.CameraPosition);

	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	//TODO: Combine GBuffer textures width lighting and stuff
}

GLTexture2D* View::GetFramebufferTexture() {
	return this->framebuffer->GetAttachedTexture(GL_COLOR_ATTACHMENT0);
}

void View::BindLightsToLightingShader(Scene* scene) {
	//Point Lights
	std::vector<PointLight*> *pointLights = scene->GetPointLights();
	for (int i = 0; i < pointLights->size(); i++) {
		if (i > 32) {
			std::cout << "VIEW::Too many point lights" << std::endl;
			break;
		}

		PointLight* pointLight = pointLights->at(i);
		std::string index = std::to_string(i);

		this->lightingShader->SetVec3("pointLights[" + index + "].color", pointLight->GetColor());

		this->lightingShader->SetFloat("pointLights[" + index + "].constant", pointLight->GetConstant());
		this->lightingShader->SetFloat("pointLights[" + index + "].linear", pointLight->GetLinear());
		this->lightingShader->SetFloat("pointLights[" + index + "].quadratic", pointLight->GetQuadratic());

		this->lightingShader->SetFloat("pointLights[" + index + "].specularStrength", pointLight->GetSpecularStrength());

		//WorldSpace
		this->lightingShader->SetVec3("pointLights[" + index + "].position", pointLight->GetPosition());
	}

	//Spot Lights
	std::vector<SpotLight*> *spotLights = scene->GetSpotLights();
	for (int i = 0; i < spotLights->size(); i++) {
		if (i > 32) {
			std::cout << "VIEW::Too many point lights" << std::endl;
			break;
		}

		SpotLight* spotLight = spotLights->at(i);
		std::string index = std::to_string(i);

		this->lightingShader->SetVec3("spotLights[" + index + "].color", spotLight->GetColor());

		this->lightingShader->SetVec3("spotLights[" + index + "].position", spotLight->GetPosition());
		this->lightingShader->SetVec3("spotLights[" + index + "].direction", spotLight->GetDirection());

		this->lightingShader->SetFloat("spotLights[" + index + "].openingAngle", spotLight->GetOpeningAngle());

		this->lightingShader->SetFloat("spotLights[" + index + "].constant", spotLight->GetConstant());
		this->lightingShader->SetFloat("spotLights[" + index + "].linear", spotLight->GetLinear());
		this->lightingShader->SetFloat("spotLights[" + index + "].quadratic", spotLight->GetQuadratic());

		this->lightingShader->SetFloat("spotLights[" + index + "].specularStrength", spotLight->GetSpecularStrength());
	}
}