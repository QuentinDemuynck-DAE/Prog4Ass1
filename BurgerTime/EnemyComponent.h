#pragma once
#include "Component.h"
#include "EnemyState.h"

class EnemyComponent : public Component
{
public:

	EnemyComponent(dae::GameObject& owner);
	virtual ~EnemyComponent() = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;
	void Update(float deltaTime) override;
	void HandleInput(const Event event);
	void SetState(std::unique_ptr<EnemyState> newState);
private:
	std::unique_ptr<EnemyState> m_CurrentState;
};

