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
#include "PlaySoundCommand.h"
#include "ServiceLocator.h"
#include <glm.hpp> 

#include "AiController.h"
#include "DebugPositionCommand.h"
#include "GamePadController.h"
#include "KeyboardControllerComponent.h"
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


	auto mapComponent = map->GetComponent<dae::MapComponent>();

	// Spawn ingredients
	for (auto position : mapComponent->GetTopBunSpawnPositions())
	{
		auto topBun = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 6 });
		scene.Add(topBun);
	}

	for (auto position : mapComponent->GetBottomBunSpawnPositions())
	{
		auto botBun = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 7 });
		scene.Add(botBun);
	}

	for (auto position : mapComponent->GetCheeseSpawnPositions())
	{
		auto cheese = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 8 });
		scene.Add(cheese);
	}

	for (auto position : mapComponent->GetMeatSpawnPositions())
	{
		auto meat = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 9 });
		scene.Add(meat);
	}

	for (auto position : mapComponent->GetTomateSpawnPositions())
	{
		auto tomato = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 10 });
		scene.Add(tomato);
	}

	for (auto position : mapComponent->GetSaladSpawnPositions())
	{
		auto salad = dae::CreateIngredient(glm::vec3{ position, 0 }, { 0,0,0 }, map->GetTransform()->GetGlobalScale(), { 14, 11 });
		scene.Add(salad);
	}


	// create players
	std::vector<dae::GameObject*> players;

	for (int i{}; i < 1; i++)
	{
		auto player = dae::CreatePlayer(mapComponent, map.get(), i);
		auto gamePad{ std::make_unique<dae::GamePad>(i) };

		players.push_back(player.get());
		scene.Add(player);
		player->AddComponent<dae::GamePadControllerComponent>(gamePad.get());
		player->GetComponent<dae::GamePadControllerComponent>()->Bind(dae::Action::Debug, std::make_shared<dae::DebugPositionCommand>(player.get()), KeyState::Down);
		player->GetComponent<dae::GamePadControllerComponent>()->Bind(dae::Action::Shoot, std::make_shared<dae::ShootPepperCommand>(player.get()), KeyState::Up);

		//player->AddComponent<dae::KeyboardControllerComponent>();
		dae::InputManager::GetInstance().AddGamePad(std::move(gamePad));


	}


	auto enemy = dae::CreateEnemy(glm::vec3{ 120,320,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f }, players, *mapComponent);
	enemy->AddComponent<dae::AiController>();
	enemy->GetComponent<dae::EnemyComponent>()->SetState(std::make_unique<dae::WalkingEnemyState>());

	scene.Add(enemy);
	dae::SceneManager::GetInstance().SetActiveScene("LevelOne");

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
}

int main(int, char* []) 
{

	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}