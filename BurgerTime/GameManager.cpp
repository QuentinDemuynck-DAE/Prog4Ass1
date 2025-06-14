#include "GameManager.h"
#include "SceneCreators.h"

void dae::GameManager::SetIngredientsToWin(const int& numIngredients)
{
	m_TotalIngredientsToWin = numIngredients;
}

void dae::GameManager::ServeIngredient()
{
	m_ServedIngredients++;

	std::cout << "Ingredients served: " << m_ServedIngredients << " of the: " << m_TotalIngredientsToWin << std::endl;

	if (m_ServedIngredients >= m_TotalIngredientsToWin)
	{
		GoToNextScene();
	}
}

void dae::GameManager::AddScore(const int& score)
{
	m_Score += score;
}

void dae::GameManager::AddPlayer(std::shared_ptr<GameObject> player)
{
	m_Players.push_back(player);
}

void dae::GameManager::LoadScenes()
{
	auto& startscene = dae::SceneManager::GetInstance().CreateScene("StartScreen");
	dae::SceneManager::GetInstance().CreateScene("LevelOne");
	dae::SceneManager::GetInstance().CreateScene("LevelTwo");
	dae::SceneManager::GetInstance().CreateScene("LevelThree");
	CreateStartScreen(startscene);

	CreateLifeTimeObjects();

	dae::SceneManager::GetInstance().SetActiveSceneByIndex(0);
}

void dae::GameManager::Reset()
{
	Minigin::m_PhysicsPaused = true;

	SceneManager::GetInstance().GetSceneAtIndex(1)->RemoveAll();
	SceneManager::GetInstance().GetSceneAtIndex(2)->RemoveAll();
	SceneManager::GetInstance().GetSceneAtIndex(3)->RemoveAll();


	m_GameMode = GameMode::SOLO;
	m_TotalIngredientsToWin = INT_MAX ;
	m_ServedIngredients = 0 ;
	m_Score = 0;
	SceneManager::GetInstance().SetActiveSceneByIndex(0);



	CreateLifeTimeObjects();

	m_MarkedReset = false;

	Minigin::m_PhysicsPaused = false;

}

void dae::GameManager::GoToNextScene()
{
	if (SceneManager::GetInstance().GetActiveScene().get()->GetName() == "LevelThree")
	{
		m_MarkedReset;
		SceneManager::GetInstance().LoadNextScene();
		return;
	}
	SceneManager::GetInstance().LoadNextScene();

	auto map = m_Maps.at(SceneManager::GetInstance().GetActiveSceneIndex() -1);
	auto mapComponent = map->GetComponent<MapComponent>();

	for (int i{}; i < m_Players.size(); i++)
	{
		auto mapwalker = m_Players.at(i)->GetComponent<MapWalkerComponent>();
		auto player = m_Players.at(i)->GetComponent<PlayerComponent>();

		if (!mapwalker || !player)
			continue;

		mapwalker->SetSpawnPosition(glm::vec3{ mapComponent->GetPlayerSpawnPositions().at(i), 0 });
		player->SetState(std::make_unique<Walking>());
		mapwalker->Respawn();
	}

	auto mapwalker = m_Enemy->GetComponent<MapWalkerComponent>();
	mapwalker->SetSpawnPosition(glm::vec3{ mapComponent->GetSausageSpawnPositions().at(0), 0.0f });
	mapwalker->Respawn();

	auto enemy = m_Enemy->GetComponent<EnemyComponent>();
	enemy->SetState(std::make_unique<WalkingEnemyState>());
	

	m_TotalIngredientsToWin = mapComponent->GetTotalIngredients();
	m_ServedIngredients = 0;

}

void dae::GameManager::SetGameMode(const GameMode& gameMode)
{

	m_GameMode = gameMode;

	auto levelOne = SceneManager::GetInstance().GetSceneAtIndex(1);
	auto levelTwo = SceneManager::GetInstance().GetSceneAtIndex(2);
	auto levelThree = SceneManager::GetInstance().GetSceneAtIndex(3);

	std::vector<std::shared_ptr<GameObject>> players;
	std::vector<std::shared_ptr<GameObject>> enemies;


	switch (gameMode)
	{
	case GameMode::SOLO:
		players.push_back(m_Players.at(0));
		break;
	case GameMode::VERSUS:
		players.push_back(m_Players.at(0));
		enemies.push_back(m_Enemy);
		break;
	case GameMode::COOP:
		players = m_Players;
		break;
	}

	CreateFirstLevel(*levelOne, m_Maps.at(0), players, enemies);
	CreateFirstLevel(*levelTwo, m_Maps.at(1), players, enemies);
	CreateFirstLevel(*levelThree, m_Maps.at(2), players, enemies);

	GoToNextScene();
}

void dae::GameManager::EliminatePlayer(GameObject& gameObject)
{
	switch (m_GameMode)
	{
	case GameMode::SOLO:
		m_MarkedReset = true;
		break;
	case GameMode::COOP:
		++m_EliminatedPlayers;
		if (m_EliminatedPlayers == 2)
		{
			m_MarkedReset = true;
			return;
		}
		gameObject.MarkDestroy();
		gameObject.GetComponent<MapWalkerComponent>()->DisableBoundaries();
		gameObject.GetTransform()->SetLocalPosition(glm::vec3{ 9999 , 9999 ,9999 }); // Somehow my enemies were still looking for him :(
		break;
	case GameMode::VERSUS:
		m_MarkedReset = true;
		break;

	}
}

void dae::GameManager::CreateLifeTimeObjects()
{

	m_Maps.clear();
	m_Players.clear();

	m_Maps.push_back(CreateMap("levelOne"));
	m_Maps.push_back(CreateMap("levelTwo"));
	m_Maps.push_back(CreateMap("levelThree"));


	m_Players.push_back(CreatePlayer(m_Maps.at(0).get()->GetComponent<MapComponent>(), m_Maps.at(0).get(), 0));
	m_Players.push_back(CreatePlayer(m_Maps.at(0).get()->GetComponent<MapComponent>(), m_Maps.at(0).get(), 1));

	m_Players.at(0)->AddComponent<GamePadControllerComponent>(InputManager::GetInstance().GetGamePadAtIndex(0));

	m_Players.at(1)->AddComponent<GamePadControllerComponent>(InputManager::GetInstance().GetGamePadAtIndex(1));
	m_Players.at(1)->AddComponent<KeyboardControllerComponent>();


	for (auto players : m_Players)
	{
		for (auto controllers : players->GetComponentsDerived<ControllerComponent>())
		{
			controllers->Bind(Action::Shoot, std::make_shared<ShootPepperCommand>(players.get()));
		}
	}


	auto mapComp = m_Maps[0]->GetComponent<MapComponent>();
	glm::vec3 spawnPos{ mapComp->GetSausageSpawnPositions()[0], 0.0f };

	m_Enemy = CreateEnemy(
		spawnPos,
		{ 0,0,0 },
		{ 2,2,2 },
		m_Players,
		*mapComp,
		"Enemies/sausage.png",
		100
	);

	m_Enemy->AddComponent<GamePadControllerComponent>(InputManager::GetInstance().GetGamePadAtIndex(1));
	m_Enemy->AddComponent<KeyboardControllerComponent>();


}
