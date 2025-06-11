#include "FallingState.h"

#include "GameObject.h"
#include "RigidbodyComponent.h"

void dae::FallingState::HandleInput(dae::GameObject& gameObject, const Event& event)
{
	event;
	gameObject;
}

void dae::FallingState::OnEnter(dae::GameObject& gameObject)
{
	if (gameObject.HasComponent<RigidbodyComponent>())
	{
		m_Rigidbody = gameObject.GetComponent<RigidbodyComponent>();
	}
}

void dae::FallingState::OnExit(dae::GameObject& gameObject)
{
	IngredientState::OnExit(gameObject);
}

void dae::FallingState::Update(dae::GameObject&, float)
{
	if (m_Rigidbody)
	{
		m_Rigidbody->AddVelocity(m_Speed);
	}
}
