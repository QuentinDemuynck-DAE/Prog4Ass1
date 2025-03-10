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
#include "TrashTheCacheComponent.h"
#include "GamePad.h"
#include "InputManager.h"

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

	auto textureOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,20,0 });
	auto textureTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,10,0 });

	textureOne->AddComponent<Texture2DComponent>("BurgerMan.png");
	textureTwo->AddComponent<Texture2DComponent>("BurgerMan.png");

	const float speed{ 1.f };
	//Connect controllers
	for (int i = 0; i < g_maxControllers; i++)
	{
		auto gamePad{ std::make_unique<GamePad>(i) };
		// Bind commands for all directions
		gamePad->BindCommand(GamePad::Button::DPadUp, std::make_shared<MoveTransformCommand>(textureOne.get(), 0.f, -speed), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadDown, std::make_shared<MoveTransformCommand>(textureOne.get(), 0.f, speed), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadLeft, std::make_shared<MoveTransformCommand>(textureOne.get(), -speed, 0.f), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadRight, std::make_shared<MoveTransformCommand>(textureOne.get(), speed, 0.f), KeyState::Pressed);
		dae::InputManager::GetInstance().ConnectGamePad(std::move(gamePad));
	}

	dae::InputManager::GetInstance().BindCommand(SDLK_w, std::make_shared<MoveTransformCommand>(textureTwo.get(), 0.f, -speed), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_s, std::make_shared<MoveTransformCommand>(textureTwo.get(), 0.f, speed), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_a, std::make_shared<MoveTransformCommand>(textureTwo.get(), -speed, 0.f), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_d, std::make_shared<MoveTransformCommand>(textureTwo.get(), speed, 0.f), KeyState::Pressed);

	scene.Add(textureOne);
	scene.Add(textureTwo);


}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}