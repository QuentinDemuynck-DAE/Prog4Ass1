#pragma once
#include <string>
#include <vector>

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

		void Reset();

		void SetGameMode(const GameMode& gameMode)
		{
			m_GameMode = gameMode;
		}
	private:

		GameMode m_GameMode = GameMode::SOLO;
		int m_TotalIngredientsToWin{ INT_MAX };
		int m_ServedIngredients{ 0 };
		int m_Score{ 0 };
		std::vector<std::shared_ptr<GameObject>> m_Players;
		std::vector<std::shared_ptr<GameObject>> m_Maps;
		std::vector<std::shared_ptr<GameObject>> m_Enemies;
	};

}
