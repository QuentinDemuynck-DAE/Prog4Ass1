#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "AnimatedSpriteComponent.h"
#include "Collision.h"

#include "CollisionComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Texture2DComponent.h"
#include "FPSComponent.h"
#include "Subject.h"
#include "ScoreObserver.h"
#include "ScoreComponent.h"
#include "Commands.h"
#include "EnemyComponent.h"
#include "EnemyObserver.h"
#include "PatrolState.h"
#include "PlaySoundCommand.h"
#include "ServiceLocator.h"
#include <glm.hpp> 

#include "DebugPositionCommand.h"
#include "MapComponent.h"
#include "SVGParser.h"
#include "Prototypes.h"


void load()
{

	auto noisySoundSystem = std::make_unique<NoisySoundSystem>();
	ServiceLocator::ProvideSoundSystem(std::make_unique<DebugSoundSystem>(std::move(noisySoundSystem)));
	auto& scene = dae::SceneManager::GetInstance().CreateScene("LevelOne");

	// Add background
	auto go = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 });
	go->AddComponent<Texture2DComponent>("background.tga");
	scene.Add(go);

	// Add logo
	go = std::make_shared<dae::GameObject>(glm::vec3{ 216,180,0 });
	go->AddComponent<Texture2DComponent>("logo.tga");
	scene.Add(go);

	auto map = dae::CreateMap("levelOne");
	scene.Add(map);


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

	auto smallerFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);



	// create players
	std::vector<dae::GameObject*> players;

	for (int i{}; i < g_maxControllers; i++)
	{
		auto player = dae::CreatePlayer(map->GetComponent<dae::MapComponent>(), map.get(), i);

		players.push_back(player.get());
		scene.Add(player);
	}


	auto enemy = dae::CreateEnemy(glm::vec3{ 20,110,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f }, players);

	scene.Add(enemy);

	//scene.Add(livesPlayerOne);
	//scene.Add(livesPlayerTwo);
	//
	//scene.Add(scorePlayerOne);
	//scene.Add(scorePlayerTwo);


	//auto livesPlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,80,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//auto scorePlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,95,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//
	//auto livesPlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,110,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//auto scorePlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,125,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//
	//livesPlayerOne->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);
	//livesPlayerTwo->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);
	//
	//scorePlayerOne->AddComponent<dae::TextComponent>("Score: 0", smallerFont);
	//scorePlayerTwo->AddComponent<dae::TextComponent>("Score: 0", smallerFont);
	//
	//auto healthObserverOne = std::make_shared<HealthObserver>(livesPlayerOne->GetComponent<dae::TextComponent>());
	//auto healthObserverTwo = std::make_shared<HealthObserver>(livesPlayerTwo->GetComponent<dae::TextComponent>());
	//
	//auto scoreObserverOne = std::make_shared<ScoreObserver>(scorePlayerOne->GetComponent<dae::TextComponent>());
	//auto scoreObserverTwo = std::make_shared<ScoreObserver>(scorePlayerTwo->GetComponent<dae::TextComponent>());

	//textureOne.get()->GetSubject()->AddObserver(healthObserverOne);
	//textureTwo.get()->GetSubject()->AddObserver(healthObserverTwo);
	//
	//textureOne.get()->GetSubject()->AddObserver(scoreObserverOne);
	//textureTwo.get()->GetSubject()->AddObserver(scoreObserverTwo);

	//const float speed{ 100.f };

	//dae::InputManager::GetInstance().BindCommand(SDLK_w, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), 0.f, -speed * 2.f), KeyState::Pressed);
	//dae::InputManager::GetInstance().BindCommand(SDLK_s, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), 0.f, speed * 2.f), KeyState::Pressed);
	//dae::InputManager::GetInstance().BindCommand(SDLK_a, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), -speed * 2.f, 0.f), KeyState::Pressed);
	//dae::InputManager::GetInstance().BindCommand(SDLK_d, std::make_shared<dae::MoveTransformCommand>(textureTwo.get(), speed * 2.f, 0.f), KeyState::Pressed);
	//dae::InputManager::GetInstance().BindCommand(SDLK_c, std::make_shared<dae::ShootPepperCommand>(textureTwo.get()), KeyState::Up);
	//dae::InputManager::GetInstance().BindCommand(SDLK_z, std::make_shared<dae::ScoreCommand>(textureTwo.get()), KeyState::Up);
	//dae::InputManager::GetInstance().BindCommand(SDLK_x, std::make_shared<dae::ScoreCommand>(textureTwo.get(), 100), KeyState::Up);
	//dae::InputManager::GetInstance().BindCommand(SDLK_p, std::make_shared<dae::DebugPositionCommand>(textureTwo.get()), KeyState::Up);
	//
	//
	//dae::InputManager::GetInstance().BindCommand(SDLK_SPACE, std::make_shared<dae::PlaySoundCommand>("../Data/Audio/Start.mp3",10), KeyState::Up);


	dae::SceneManager::GetInstance().SetActiveScene("LevelOne");
}

int main(int, char* []) 
{

	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}