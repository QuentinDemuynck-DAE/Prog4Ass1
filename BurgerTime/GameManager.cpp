#include "GameManager.h"

void dae::GameManager::SetIngredientsToWin(const int& numIngredients)
{
	m_TotalIngredientsToWin = numIngredients;
}

void dae::GameManager::ServeIngredient()
{
	m_ServedIngredients++;

	if (m_ServedIngredients > m_TotalIngredientsToWin)
	{
		m_TotalIngredientsToWin = INT_MAX;
		m_ServedIngredients = 0;

		SceneManager::GetInstance().LoadNextScene();
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

void dae::GameManager::Reset()
{

}
