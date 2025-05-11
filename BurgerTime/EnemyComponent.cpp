#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(dae::GameObject& owner)
	:Component(owner)
{
}

void EnemyComponent::Update(float deltaTime)
{
	if (m_CurrentState) m_CurrentState->Update(GetOwner(), deltaTime);
}

void EnemyComponent::HandleInput(const Event event)
{
	if (m_CurrentState) m_CurrentState->HandleInput(GetOwner(), event);
}

void EnemyComponent::SetState(std::unique_ptr<EnemyState> newState)
{
	if (m_CurrentState) m_CurrentState->OnExit(GetOwner());
	m_CurrentState = std::move(newState);
	if (m_CurrentState) m_CurrentState->OnEnter(GetOwner());
}
