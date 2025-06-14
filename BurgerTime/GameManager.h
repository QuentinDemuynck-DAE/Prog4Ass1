#pragma once
#include <string>
#include <vector>

#include "Prototypes.h"
#include "SceneManager.h"
#include "Singleton.h"

namespace dae
{
	enum class GameMode
	{
		SOLO,
		COOP,
		VERSUS
	};


	class GameManager : public dae::Singleton<GameManager> //Manges game state
	{
	public:

		void SetIngredientsToWin(const int& numIngredients);
		void ServeIngredient();
		void AddScore(const int& score);
		void AddPlayer(std::shared_ptr<GameObject> player);

		void LoadScenes();
		void Reset();

		void GoToNextScene();
		void SetGameMode(const GameMode& gameMode);

		void EliminatePlayer(GameObject& gameObject);

		std::vector<std::shared_ptr<GameObject>> GetActivePlayers()
		{
			return m_Players;
		}

		void SetCollisionListener(CollisionListener* colLis)
		{
			m_CollisionListener = colLis;
		}

		bool ShouldReset() { return m_MarkedReset; }

	private:
		bool m_MarkedReset{ false };
		void CreateLifeTimeObjects();

		GameMode m_GameMode = GameMode::SOLO;
		int m_TotalIngredientsToWin{ INT_MAX };
		int m_ServedIngredients{ 0 };
		int m_Score{ 0 };
		int m_EliminatedPlayers{ 0 };
		std::vector<std::shared_ptr<GameObject>> m_Players;
		std::vector<std::shared_ptr<GameObject>> m_Maps;
		std::shared_ptr<GameObject> m_Enemy;
		std::vector<std::shared_ptr<GameObject>> m_UIS;
		CollisionListener* m_CollisionListener;
	};

}
