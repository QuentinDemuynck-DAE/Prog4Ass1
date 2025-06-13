#include "EnemyFallingAlong.h"

#include "EnemyDying.h"
#include "GameObject.h"
#include "Globals.h"
#include "IngredientComponent.h"

dae::EnemyFallingAlong::EnemyFallingAlong(GameObject* ingredient)
	:m_Ingredient(ingredient)
{
	if (m_Ingredient->HasComponent<RigidbodyComponent>())
	{
		m_IngredientBody = m_Ingredient->GetComponent<RigidbodyComponent>();
	}

}

void dae::EnemyFallingAlong::HandleInput(GameObject&, const Event& event)
{
	if (event.id == make_sdbm_hash("ingredient_landed"))
	{
		if (m_EnemyComponent)
		{
			auto state = std::make_unique<EnemyDying>();
			m_EnemyComponent->SetState(std::move(state));
		}
	}
	
}

void dae::EnemyFallingAlong::OnEnter(GameObject& gameObject)
{
	if (gameObject.HasComponent<RigidbodyComponent>())
	{
		m_OwnBody = gameObject.GetComponent<RigidbodyComponent>();
	}

	if (gameObject.HasComponent<MapWalkerComponent>())
	{
		m_MapWalker = gameObject.GetComponent<MapWalkerComponent>();
		m_MapWalker->DisableBoundaries();
	}

	if (gameObject.HasComponent<EnemyComponent>())
	{
		m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
		m_EnemyComponent->SetCanHit(false);
	}
}


void dae::EnemyFallingAlong::Update(GameObject&, float)
{
	m_OwnBody->SetVelocity(m_IngredientBody->GetVelocity());
}

void dae::EnemyFallingAlong::OnExit(GameObject& )
{
	if (m_MapWalker)
	{
		m_MapWalker->EnableBoundaries();
	}
}
