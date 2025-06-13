#pragma once
#include "ButtonComponent.h"
#include "EnemyComponent.h"
#include "FPSComponent.h"
#include "GameManager.h"
#include "GamePadController.h"
#include "MenuController.h"
#include "Prototypes.h"
#include "Prototypes.h"
#include "Prototypes.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Texture2DComponent.h"
#include "WalkingEnemyState.h"
#include "Texture2DComponent.h"
#include "KeyboardControllerComponent.h"


inline void CreateFirstLevel(dae::Scene& scene , std::shared_ptr<dae::GameObject> map, std::vector<std::shared_ptr<dae::GameObject>> players, std::vector<std::shared_ptr<dae::GameObject>> additionalEnemies)
{
	scene.RemoveAll();

	// Add background
	auto go = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 });
	go->AddComponent<Texture2DComponent>("background.tga");
	scene.Add(go);

	// Add logo
	go = std::make_shared<dae::GameObject>(glm::vec3{ 216,180,0 });
	go->AddComponent<Texture2DComponent>("logo.tga");
	scene.Add(go);

	scene.Add(map);


	scene.Add(map);

	std::vector<dae::GameObject*> playersToAdd;

	for (std::shared_ptr<dae::GameObject> player : players)
	{
		scene.Add(player);
		playersToAdd.push_back(player.get());
	}
	for (auto enemy : additionalEnemies)
	{
		scene.Add(enemy);
		enemy->GetComponent<dae::EnemyComponent>()->SetPlayers(playersToAdd);
	}

	// Add text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);


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

	auto enemy = dae::CreateEnemy(glm::vec3{ 120,320,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f }, players, *mapComponent, "Enemies/Sausage.png", 100);
	enemy->AddComponent<dae::AiController>();
	enemy->GetComponent<dae::EnemyComponent>()->SetState(std::make_unique<dae::WalkingEnemyState>());

	scene.Add(enemy);
}


inline void CreateStartScreen(dae::Scene& scene)
{
	scene.RemoveAll();

	auto menuGO = std::make_shared<dae::GameObject>();
	menuGO->AddComponent<dae::MenuController>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("burger.otf", 36);



	// Add background
	auto go = std::make_shared<dae::GameObject>(glm::vec3{ 0,0,0 });
	go->AddComponent<Texture2DComponent>("background.tga");
	scene.Add(go);


	// Add text
	go = std::make_shared<dae::GameObject>(glm::vec3{ 100,50,0 });
	go->AddComponent<dae::TextComponent>("BURGER TIME!", font);
	scene.Add(go);

	auto buttonSolo = std::make_shared<dae::GameObject>(glm::vec3{ 200, 150, 0});
	buttonSolo->AddComponent<dae::TextComponent>("Solo", font);
	buttonSolo->AddComponent<dae::ButtonComponent>(std::make_unique<dae::LoadSceneCommand>(dae::GameMode::SOLO));

	auto buttonDuo = std::make_shared<dae::GameObject>(glm::vec3{ 200, 250, 0 });
	buttonDuo->AddComponent<dae::TextComponent>("Coop", font);
	buttonDuo->AddComponent<dae::ButtonComponent>(std::make_unique<dae::LoadSceneCommand>(dae::GameMode::COOP));

	auto buttonVersus = std::make_shared<dae::GameObject>(glm::vec3{ 200, 350, 0 });
	buttonVersus->AddComponent<dae::TextComponent>("VERSUS", font);
	buttonVersus->AddComponent<dae::ButtonComponent>(std::make_unique<dae::LoadSceneCommand>(dae::GameMode::VERSUS));

	auto menuCtrl = menuGO->GetComponent<dae::MenuController>();
	menuCtrl->AddButton(buttonSolo->GetComponent<dae::ButtonComponent>());
	menuCtrl->AddButton(buttonDuo->GetComponent<dae::ButtonComponent>());
	menuCtrl->AddButton(buttonVersus->GetComponent<dae::ButtonComponent>());


	for (int i{}; i < g_maxControllers; i++)
	{
		auto controller = std::make_shared<dae::GameObject>();


		controller->AddComponent<dae::KeyboardControllerComponent>();
		controller->AddComponent<dae::GamePadControllerComponent>(dae::InputManager::GetInstance().GetGamePadAtIndex(i));

		for (auto controllerComp : controller->GetComponentsDerived<dae::ControllerComponent>())
		{
			controllerComp->Bind(dae::Action::Up, std::make_shared<dae::MenuUpCommand>(menuCtrl));
			controllerComp->Bind(dae::Action::Down, std::make_shared<dae::MenuDownCommand>(menuCtrl));
			controllerComp->Bind(dae::Action::Shoot, std::make_shared<dae::MenuClickCommand>(menuCtrl));
		}

		scene.Add(controller);
	}
	scene.Add(menuGO);
	scene.Add(buttonDuo);
	scene.Add(buttonSolo);
	scene.Add(buttonVersus);
}