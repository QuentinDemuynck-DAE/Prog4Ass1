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
#include "TransformComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();

	auto textureComponent = std::make_unique<Texture2DComponent>("background.tga");
	auto transformComponent = std::make_unique<dae::TransformComponent>(glm::vec3{0,0,0});

	go->AddComponent<Texture2DComponent>(std::move(textureComponent));
	go->AddComponent<dae::TransformComponent>(std::move(transformComponent));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();

	textureComponent = std::make_unique<Texture2DComponent>("logo.tga");
	transformComponent = std::make_unique<dae::TransformComponent>(glm::vec3{ 216,180,0 });
	go->AddComponent<Texture2DComponent>(std::move(textureComponent));
	go->AddComponent<dae::TransformComponent>(std::move(transformComponent));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<dae::TextComponent>("Programming 4 Assignment", font);
	transformComponent = std::make_unique<dae::TransformComponent>(glm::vec3{ 80,20,0 });

	go->AddComponent<dae::TextComponent>(std::move(to));
	go->AddComponent<dae::TransformComponent>(std::move(transformComponent));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	transformComponent = std::make_unique<dae::TransformComponent>(glm::vec3{ 5,20,0 });
	auto fpsComponent = std::make_unique<FPSComponent>(font);
	go->AddComponent<dae::TransformComponent>(std::move(transformComponent));
	go->AddComponent<FPSComponent>(std::move(fpsComponent));

	scene.Add(go);


}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}