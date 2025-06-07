#include "ShootPepperCommand.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include "PlayerComponent.h"
#include "Texture2DComponent.h"

dae::ShootPepperCommand::ShootPepperCommand(dae::GameObject* gameObject)
{
	m_OwningObj = gameObject;
	m_PlayerComponent = gameObject->GetComponent<PlayerComponent>();
}

void dae::ShootPepperCommand::Execute()
{
	if (m_PlayerComponent->GetPepperCooldown() < m_PlayerComponent->PEPPER_COOLDOWN) return;

	if (!m_PlayerComponent->GetIsShooting())
	{
		auto pepper = std::make_shared<GameObject>(glm::vec3{ 24, 24,24 });
		pepper->AddComponent<Texture2DComponent>("pepper.png");
		pepper->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), dae::CollisionLayers::SALT, dae::CollisionLayers::ENEMY , glm::vec2{ 8, 8 }, glm::vec2{ 8, 8 }, true, false);
		pepper->SetParent(m_OwningObj);
		m_PlayerComponent->SetIsShooting(true);
		m_PlayerComponent->SetPepper(pepper.get());
	}
}
