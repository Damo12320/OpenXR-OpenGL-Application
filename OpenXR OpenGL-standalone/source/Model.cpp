#include "Model.h"

void Model::Start() {
	std::unique_ptr<Scene> scene = std::make_unique<Scene>();
	Scene::SetCurrentScene(std::move(scene));

	Scene* currentScene = Scene::GetCurrentScene();


	GameSceneBuilder::BuildArena(currentScene);
	GameSceneBuilder::AddSkybox(currentScene);
	GameSceneBuilder::AddTargets(currentScene);
	GameSceneBuilder::AddBall(currentScene);
	GameSceneBuilder::AddControllers(currentScene);

	//After all gameObjects are added to the scene, start them
	currentScene->Start();
	std::cout << "Model Started" << "\n";
};

void Model::Update(float deltaTime) {
	Scene::GetCurrentScene()->Update(deltaTime);
};