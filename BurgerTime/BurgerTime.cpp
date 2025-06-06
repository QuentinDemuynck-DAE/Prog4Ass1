#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Collision.h"

#include "CollisionComponent.h"
#include "CollisionListener.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Texture2DComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "GamePad.h"
#include "InputManager.h"
#include "RigidbodyComponent.h"
#include "Command.h"
#include "HealthComponent.h"
#include "LivesComponent.h"
#include "HealthObserver.h"
#include "Subject.h"
#include "ScoreObserver.h"
#include "ScoreComponent.h"
#include "Commands.h"
#include "EnemyComponent.h"
#include "EnemyObserver.h"
#include "PatrolState.h"
#include "PlaySoundCommand.h"
#include "ServiceLocator.h"
#include "ShootPepperCommand.h"
#include "PlayerComponent.h"


void load()
{

	auto noisySoundSystem = std::make_unique<NoisySoundSystem>();
	ServiceLocator::ProvideSoundSystem(std::make_unique<DebugSoundSystem>(std::move(noisySoundSystem)));
	auto& scene = dae::SceneManager::GetInstance().CreateScene("LevelOne");

	auto map = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	map->AddComponent<Texture2DComponent>("levelOne.png");
	scene.Add(map);
	// Add background
	auto go = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 });
	//go->AddComponent<Texture2DComponent>("background.tga");
	//scene.Add(go);

	// Add logo
	go = std::make_shared<dae::GameObject>(glm::vec3{ 216,180,0 });
	go->AddComponent<Texture2DComponent>("logo.tga");
	scene.Add(go);

	// Add text
	go = std::make_shared<dae::GameObject>(glm::vec3{ 80,20,0 });
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	scene.Add(go);

	//Tutorial
	go = std::make_shared<dae::GameObject>(glm::vec3{ 100,80,0 });
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	go->AddComponent<dae::TextComponent>("Press space to play sound!", font);
	scene.Add(go);

	// Add FPS counter
	go = std::make_shared<dae::GameObject>(glm::vec3{ 10,20,0 });
	go->AddComponent<dae::TextComponent>("60", font);
	go->AddComponent<FPSComponent>();
	scene.Add(go);

	auto textureOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,200,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	auto textureTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,10,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });

	textureOne->AddComponent<RigidbodyComponent>();
	textureTwo->AddComponent<RigidbodyComponent>(20.f, 10.f, 0.1f);


	textureOne->AddComponent<Texture2DComponent>("BurgerMan.png");
	textureTwo->AddComponent<Texture2DComponent>("BurgerMan.png");

	textureOne->AddComponent<HealthComponent>(1);
	textureTwo->AddComponent<HealthComponent>(1);

	textureOne->AddComponent<LivesComponent>(5);
	textureTwo->AddComponent<LivesComponent>(5);

	textureOne->AddComponent<ScoreComponent>();
	textureTwo->AddComponent<ScoreComponent>();

	auto smallerFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);


	auto livesPlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,80,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	auto scorePlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,95,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });

	auto livesPlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,110,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	auto scorePlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,125,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });

	livesPlayerOne->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);
	livesPlayerTwo->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);

	scorePlayerOne->AddComponent<dae::TextComponent>("Score: 0", smallerFont);
	scorePlayerTwo->AddComponent<dae::TextComponent>("Score: 0", smallerFont);

	auto healthObserverOne = std::make_shared<HealthObserver>(livesPlayerOne->GetComponent<dae::TextComponent>());
	auto healthObserverTwo = std::make_shared<HealthObserver>(livesPlayerTwo->GetComponent<dae::TextComponent>());

	auto scoreObserverOne = std::make_shared<ScoreObserver>(scorePlayerOne->GetComponent<dae::TextComponent>());
	auto scoreObserverTwo = std::make_shared<ScoreObserver>(scorePlayerTwo->GetComponent<dae::TextComponent>());

	textureOne.get()->GetSubject()->AddObserver(healthObserverOne);
	textureTwo.get()->GetSubject()->AddObserver(healthObserverTwo);

	textureOne.get()->GetSubject()->AddObserver(scoreObserverOne);
	textureTwo.get()->GetSubject()->AddObserver(scoreObserverTwo);

	textureOne->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), glm::vec2{ 8, 8 }, glm::vec2{ 8, 8 }, true, false);
	textureTwo->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), glm::vec2{ 8, 8 }, glm::vec2{ 8, 8 }, true, false);

	textureOne->AddComponent<PlayerComponent>();
	textureTwo->AddComponent<PlayerComponent>();




	auto enemyObserver = std::make_shared<EnemyObserver>();
	auto enemy = std::make_shared<dae::GameObject>(glm::vec3{ 20,110,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });

	std::vector<dae::GameObject*> players;
	players.push_back(textureOne.get());
	players.push_back(textureTwo.get());

	enemy->AddComponent<Texture2DComponent>("EnemyOne.png");
	enemy->GetSubject()->AddObserver(enemyObserver);
	enemy->AddComponent<EnemyComponent>(players);
	enemy->GetComponent<EnemyComponent>()->SetState(std::make_unique<PatrolState>());
	enemy->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), glm::vec2{ 8, 8 }, glm::vec2{ 8, 8 }, true, false);

	scene.Add(enemy);

	scene.Add(livesPlayerOne);
	scene.Add(livesPlayerTwo);

	scene.Add(scorePlayerOne);
	scene.Add(scorePlayerTwo);


	const float speed{ 1.f };
	//Connect controllers
	for (int i = 0; i < g_maxControllers; i++)
	{
		auto gamePad{ std::make_unique<GamePad>(i) };
		// Bind commands for all directions
		gamePad->BindCommand(GamePad::Button::DPadUp, std::make_shared<dae::MoveTransformCommand>(textureOne.get(), 0.f, -speed), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadDown, std::make_shared<dae::MoveTransformCommand>(textureOne.get(), 0.f, speed), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadLeft, std::make_shared<dae::MoveTransformCommand>(textureOne.get(), -speed, 0.f), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::DPadRight, std::make_shared<dae::MoveTransformCommand>(textureOne.get(), speed, 0.f), KeyState::Pressed);
		gamePad->BindCommand(GamePad::Button::ButtonX, std::make_shared<dae::ShootPepperCommand>(textureOne.get()), KeyState::Up);
		gamePad->BindCommand(GamePad::Button::ButtonA, std::make_shared<dae::ScoreCommand>(textureOne.get()), KeyState::Up);
		gamePad->BindCommand(GamePad::Button::ButtonB, std::make_shared<dae::ScoreCommand>(textureOne.get(), 100), KeyState::Up);

		dae::InputManager::GetInstance().ConnectGamePad(std::move(gamePad));
	}

	dae::InputManager::GetInstance().BindCommand(SDLK_w, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), 0.f, -speed * 2.f), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_s, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), 0.f, speed * 2.f), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_a, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), -speed * 2.f, 0.f), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_d, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), speed * 2.f, 0.f), KeyState::Pressed);
	dae::InputManager::GetInstance().BindCommand(SDLK_c, std::make_shared<dae::ShootPepperCommand>(textureTwo.get()), KeyState::Up);
	dae::InputManager::GetInstance().BindCommand(SDLK_z, std::make_shared<dae::ScoreCommand>(textureTwo.get()), KeyState::Up);
	dae::InputManager::GetInstance().BindCommand(SDLK_x, std::make_shared<dae::ScoreCommand>(textureTwo.get(), 100), KeyState::Up);

	dae::InputManager::GetInstance().BindCommand(SDLK_SPACE, std::make_shared<dae::PlaySoundCommand>("../Data/Audio/Start.mp3",10), KeyState::Up);



	scene.Add(textureOne);
	scene.Add(textureTwo);

	dae::SceneManager::GetInstance().SetActiveScene("LevelOne");
}

int main(int, char* []) 
{


	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}