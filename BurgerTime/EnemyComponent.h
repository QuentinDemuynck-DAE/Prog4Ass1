#pragma once
#include "Component.h"
#include "EnemyState.h"

class GameObject;

class EnemyComponent : public Component
{
public:

	EnemyComponent(dae::GameObject& owner, std::vector<dae::GameObject*> players);
	virtual ~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;
	void Update(float deltaTime) override;
	void HandleInput(const Event event);
	void SetState(std::unique_ptr<EnemyState> newState);

	void SeekPlayer();

private:
	const float SEEK_DISTANCE = 2000.0f;
	std::unique_ptr<EnemyState> m_CurrentState;
	std::vector<dae::GameObject*> m_Players;
	dae::GameObject* m_ChasingPlayer;
};

