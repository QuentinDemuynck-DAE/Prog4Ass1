#pragma once
#include <memory>

#include "AnimatedSpriteComponent.h"
#include "CollisionComponent.h"
#include "Commands.h"
#include "DebugPositionCommand.h"
#include "GameObject.h"
#include "GamePad.h"
#include "IngredientComponent.h"
#include "IngredientObserver.h"
#include "InputManager.h"
#include "LivesComponent.h"
#include "MapComponent.h"
#include "MapTileWalkerObserver.h"
#include "MapWalkerComponent.h"
#include "Minigin.h"
#include "PlayerComponent.h"
#include "PlayerObserver.h"
#include "ShootPepperCommand.h"
#include "Subject.h"
#include "RigidbodyComponent.h"

// Not the prototypes design pattern! just felt like the name could match this aswell
namespace dae
{
	using GameObjectPtr = std::shared_ptr<GameObject>;

	inline GameObjectPtr CreatePlayer(MapComponent* map,GameObject* actualMap, int playerIndex = 0)
	{
		const auto& spawns = map->GetPlayerSpawnPositions();
		if (playerIndex < 0 || playerIndex >= static_cast<int>(spawns.size()))
		{
			throw std::out_of_range{
				"CreatePlayer: playerIndex "
				+ std::to_string(playerIndex)
				+ " is out of range (0–"
				+ std::to_string(spawns.size() - 1) + ")."
			};
		}

		auto player = std::make_shared<dae::GameObject>(glm::vec3{spawns.at(playerIndex),0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
		player->AddComponent<RigidbodyComponent>(80.f, 10.f, 0.1f);
		player->AddComponent<AnimatedSpriteComponent>("allAssets.png", 0, 2, glm::ivec2{ 16 , 16 }, glm::ivec2{ 10, 15 }, 0.5f);
		player->AddComponent<LivesComponent>(5);

		dae::CollisionLayers playerColidesWith
		{
			dae::CollisionLayers::ALL & ~dae::CollisionLayers::PLAYER

		};

		dae::CollisionLayers playerOwnLayer
		{
			dae::CollisionLayers::PLAYER
		};

		player->AddComponent<dae::CollisionComponent>(*dae::Minigin::physicsWorld.get(), playerOwnLayer, playerColidesWith, glm::vec2{ 6, 8 }, glm::vec2{ 2, 0 }, true, false);
		player->AddComponent<dae::MapWalkerComponent>(glm::vec3{ spawns.at(playerIndex),0 }, *actualMap->GetComponent<dae::MapComponent>());

		auto mapWalkerObs = std::make_shared<MapTileWalkerObserver>(player->GetComponent<dae::MapWalkerComponent>());

		player->GetSubject()->AddObserver(mapWalkerObs);

		player->AddComponent<PlayerComponent>();
		auto playerObserver = std::make_shared<PlayerObserver>();

		player->GetSubject()->AddObserver(playerObserver);

		auto gamePad{ std::make_unique<dae::GamePad>(playerIndex)};

		gamePad->BindCommand(dae::GamePad::Button::ButtonX, std::make_shared<dae::ShootPepperCommand>(player.get()), KeyState::Up);
		gamePad->BindCommand(dae::GamePad::Button::ButtonA, std::make_shared<dae::ScoreCommand>(player.get()), KeyState::Up);
		gamePad->BindCommand(dae::GamePad::Button::ButtonB, std::make_shared<dae::ScoreCommand>(player.get(), 100), KeyState::Up);
		gamePad->BindCommand(dae::GamePad::Button::LeftTrigger, std::make_shared<dae::DebugPositionCommand>(player.get()), KeyState::Up);

		player->GetComponent<PlayerComponent>()->BindGamepad(gamePad.get());
		dae::InputManager::GetInstance().ConnectGamePad(std::move(gamePad));

		return player;
	}

	inline GameObjectPtr CreateEnemy(glm::vec3 position , glm::vec3 rotation, glm::vec3 scale,std::vector<GameObject*> players)
	{
		dae::CollisionLayers enemyCollidesWith
		{
			dae::CollisionLayers::PLAYER | dae::CollisionLayers::SALT
		};


		auto enemy = std::make_shared<GameObject>(position, rotation, scale);
		enemy->AddComponent<Texture2DComponent>("EnemyOne.png");
		auto enemyObserver = std::make_shared<dae::EnemyObserver>();
		enemy->GetSubject()->AddObserver(enemyObserver);
		enemy->AddComponent<dae::EnemyComponent>(players);
		enemy->GetComponent<dae::EnemyComponent>()->SetState(std::make_unique<dae::PatrolState>());
		enemy->AddComponent<dae::CollisionComponent>(*dae::Minigin::physicsWorld.get(), dae::CollisionLayers::ENEMY, enemyCollidesWith, glm::vec2{ 6, 8 }, glm::vec2{ 2, 0 }, true, false);

		return enemy;
	}
	

	inline GameObjectPtr CreateMap(std::string mapName)
	{
		glm::vec3 mapScale = glm::vec3{ 2.0f, 2.0f,2.0f };
		glm::vec3 mapSize = glm::vec3{ 210, 210, 0 };
		mapSize *= mapScale;
		glm::vec3 mapOffset{};
		mapOffset.x = (g_windowWidth - mapSize.x) / 2;
		mapOffset.y = (g_windowHeight - mapSize.y) / 2;

		std::string texturePath = mapName + ".png";
		std::string dataPath = mapName + ".json";

		auto map = std::make_shared<dae::GameObject>(mapOffset, glm::vec3{ 0,0,0 }, mapScale);
		map->AddComponent<Texture2DComponent>(texturePath);
		map->AddComponent<dae::MapComponent>(*dae::Minigin::physicsWorld.get(), dataPath, glm::vec2{ 0,0 });

		return map;
		
	}

	// 6 = top, 7 = bot , 8 = cheese, 9 = meat, 10 = tomato, 11 = salad
	inline GameObjectPtr CreateIngredient(glm::vec3 position , glm::vec3 rotation, glm::vec3 scale,glm::ivec2 startRowIndex, glm::ivec2 srcSize = { 8 , 8 })
	{
		glm::ivec4 startPos{ startRowIndex, srcSize };
		auto ingredient = std::make_shared<GameObject>(position, rotation, scale);
		ingredient->AddComponent<IngredientComponent>(startPos);
		ingredient->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), dae::CollisionLayers::INGREDIENT, (CollisionLayers::ENEMY | CollisionLayers::MAP | CollisionLayers::PLATE | CollisionLayers::INGREDIENT) , glm::vec2{ 12, 4 }, glm::vec2{ 4, 0 }, true, false);
		ingredient->AddComponent<RigidbodyComponent>(20.0f);
		auto observer = std::make_shared<IngredientObserver>();
		ingredient->GetSubject()->AddObserver(observer);

		return ingredient;
	}
	
}
