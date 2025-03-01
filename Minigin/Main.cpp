#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Texture2DComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// Add background
	auto go = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 });
	go->AddComponent<Texture2DComponent>("background.tga");
	scene.Add(go);

	// Add logo
	go = std::make_shared<dae::GameObject>(glm::vec3{ 216,180,0 });
	go->AddComponent<Texture2DComponent>("logo.tga");
	scene.Add(go);

	// Add text
	go = std::make_shared<dae::GameObject>(glm::vec3{ 80,20,0 });
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	scene.Add(go);

	// Add FPS counter
	go = std::make_shared<dae::GameObject>(glm::vec3{ 10,20,0 });
	go->AddComponent<dae::TextComponent>("60", font);
	go->AddComponent<FPSComponent>();
	scene.Add(go);

	auto parent = std::make_shared<dae::GameObject>(glm::vec3{ 150,150,0 });
	auto textureOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,20,0 });
	auto textureTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,10,0 });
	auto textureThree = std::make_shared<dae::GameObject>(glm::vec3{ -10,-20,0 });
	auto textureFour = std::make_shared<dae::GameObject>(glm::vec3{ -20,20,0 });



	textureOne->AddComponent<RotateComponent>(180.0f, true);
	textureTwo->AddComponent<RotateComponent>(220.0f, false);
	textureThree->AddComponent<RotateComponent>(220.0f, false);
	textureFour->AddComponent<RotateComponent>(220.0f, false);


	textureOne->AddComponent<Texture2DComponent>("BurgerMan.png");
	textureTwo->AddComponent<Texture2DComponent>("BurgerMan.png");
	textureThree->AddComponent<Texture2DComponent>("BurgerMan.png");
	textureFour->AddComponent<Texture2DComponent>("BurgerMan.png");



	scene.Add(parent);

	scene.Add(textureOne);
	textureOne->SetParent(parent.get());

	scene.Add(textureTwo);
	textureTwo->SetParent(textureOne.get());

	scene.Add(textureThree);
	textureThree->SetParent(textureOne.get());

	scene.Add(textureFour);
	textureFour->SetParent(textureOne.get());
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}