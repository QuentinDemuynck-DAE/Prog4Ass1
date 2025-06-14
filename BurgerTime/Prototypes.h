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
#include "Walking.h"
#include "WalkingEnemyState.h"
#include "EnemyObserver.h"
#include "Texture2DComponent.h"
#include "ScoreComponent.h"

#include "EnemyComponent.h"
#include "HealthObserver.h"
#include "PepperObserver.h"

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
		player->AddComponent<RigidbodyComponent>(100.f, 10.f, 0.3f);
		player->AddComponent<AnimatedSpriteComponent>("allAssets.png", 0, 2, glm::ivec2{ 16 , 16 }, glm::ivec2{ 10, 15 }, 0.5f);
		player->AddComponent<LivesComponent>(4);

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
		player->GetComponent<PlayerComponent>()->SetState(std::make_unique<Walking>());
		auto playerObserver = std::make_shared<PlayerObserver>();

		player->GetSubject()->AddObserver(playerObserver);


		return player;
	}

	inline GameObjectPtr CreateEnemy(glm::vec3 position , glm::vec3 rotation, glm::vec3 scale,std::vector<std::shared_ptr<GameObject>> playersToAdd, MapComponent& map,std::string fileName , const int& value = 100)
	{
		dae::CollisionLayers enemyCollidesWith
		{
			dae::CollisionLayers::PLAYER | dae::CollisionLayers::SALT | dae::CollisionLayers::INGREDIENT
		};

		std::vector<GameObject*> players;

		for (auto player : playersToAdd)
		{
			players.push_back(player.get());
		}


		auto enemy = std::make_shared<GameObject>(position, rotation, scale);
		enemy->AddComponent<AnimatedSpriteComponent>(fileName, 0, 3, glm::ivec2{16,16}, glm::ivec2{2,6}, 0.3f);
		auto enemyObserver = std::make_shared<dae::EnemyObserver>();
		enemy->GetSubject()->AddObserver(enemyObserver);
		enemy->AddComponent<RigidbodyComponent>(100.f, 10.f, 0.5f);
		enemy->AddComponent<dae::EnemyComponent>(players, value);
		enemy->AddComponent<dae::CollisionComponent>(*dae::Minigin::physicsWorld.get(), dae::CollisionLayers::ENEMY, enemyCollidesWith, glm::vec2{ 5, 8 }, glm::vec2{ 3, 0 }, true, false);
		enemy->AddComponent<MapWalkerComponent>(position, map);
		auto mapWalkerObs = std::make_shared<MapTileWalkerObserver>(enemy->GetComponent<dae::MapWalkerComponent>());

		enemy->GetSubject()->AddObserver(mapWalkerObs);
		enemy->GetComponent<dae::EnemyComponent>()->SetState(std::make_unique<dae::WalkingEnemyState>());

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
		ingredient->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), (CollisionLayers::INGREDIENT), (CollisionLayers::ENEMY | CollisionLayers::MAP | CollisionLayers::PLATE | CollisionLayers::INGREDIENT) , glm::vec2{ 12, 4 }, glm::vec2{ 4, 0 }, true, false);
		ingredient->AddComponent<RigidbodyComponent>(30.0f);
		auto observer = std::make_shared<IngredientObserver>();
		ingredient->GetSubject()->AddObserver(observer);

		return ingredient;
	}

	inline GameObjectPtr CreateLivesAndPepperDisplay(float height, GameObjectPtr player)
	{
		auto smallerFont = dae::ResourceManager::GetInstance().LoadFont("burger.otf", 12);

		auto mainObj = std::make_shared<dae::GameObject>(glm::vec3{10, height, 0});
		auto lives = std::make_shared<dae::GameObject>(glm::vec3{0 , -10 ,0});
		lives->AddComponent<TextComponent>("Lifes: 4", smallerFont);
		player->GetSubject()->AddObserver(std::make_shared<HealthObserver>(lives->GetComponent<TextComponent>()));

		auto peppers = std::make_shared<dae::GameObject>(glm::vec3{ 0 , 10 ,0 });
		peppers->AddComponent<TextComponent>("Peppers: 5", smallerFont);
		player->GetSubject()->AddObserver(std::make_shared<PepperObserver>(peppers->GetComponent<TextComponent>()));

		lives->SetParent(mainObj.get());
		peppers->SetParent(mainObj.get());

		return mainObj;
	}

	inline GameObjectPtr CreateScoreDisplay()
	{
		auto smallerFont = dae::ResourceManager::GetInstance().LoadFont("burger.otf", 12);

		auto mainObj = std::make_shared<dae::GameObject>(glm::vec3{g_windowWidth / 2, 10, 0});
		mainObj->AddComponent<dae::TextComponent>("Score: 0" ,smallerFont);
		mainObj->AddComponent<dae::ScoreComponent>(mainObj->GetComponent<dae::TextComponent>());

		return mainObj;
	}
	
}
