#include "ShootPepperCommand.h"

#include "AnimatedSpriteComponent.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include "PepperComponent.h"
#include "PlayerComponent.h"
#include "RigidbodyComponent.h"
#include "Texture2DComponent.h"

dae::ShootPepperCommand::ShootPepperCommand(dae::GameObject* gameObject)
{
	m_OwningObj = gameObject;
	m_PlayerComponent = gameObject->GetComponent<PlayerComponent>();
}

void dae::ShootPepperCommand::Execute()
{
	if (m_PlayerComponent->GetPepperCooldown() < m_PlayerComponent->PEPPER_COOLDOWN) return;

	if (!m_PlayerComponent->GetIsShooting() && m_PlayerComponent->HasPeppers())
	{
		auto pepper = std::make_shared<GameObject>(glm::vec3{ 0, 0,0});
		pepper->AddComponent<AnimatedSpriteComponent>("Pepper/pepper.png",0,2,glm::ivec2{64,16}, glm::ivec2{3,1}, 0.4f );
		pepper->GetComponent<AnimatedSpriteComponent>()->Loop(true);
		pepper->AddComponent<PepperComponent>();
		pepper->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), dae::CollisionLayers::SALT, dae::CollisionLayers::ENEMY , glm::vec2{ 64, 16 }, glm::vec2{ 0, 0 }, true, false);
		pepper->SetParent(m_OwningObj);

		if (m_OwningObj->HasComponent<RigidbodyComponent>())
		{
			auto rb = m_OwningObj->GetComponent<RigidbodyComponent>();
			glm::vec2 lastVelo = rb->GetLastAddedVelocity();

			rb->Freeze();
			auto cardinal = RigidbodyComponent::GetCardinalDirection(lastVelo);
			switch (cardinal)
			{

			case RigidbodyComponent::Direction::Up:
				pepper->GetTransform()->SetLocalRotation({ glm::vec3 {90.0f ,0 ,0} });
				pepper->GetTransform()->SetLocalPosition({ glm::vec3 {32 ,0 ,0} });
				break;
				case RigidbodyComponent::Direction::Down:
				pepper->GetTransform()->SetLocalRotation({ glm::vec3 {-90.0f ,0 ,0} });
				pepper->GetTransform()->SetLocalPosition({ glm::vec3 {0 ,32 ,0} });
				break;
				case RigidbodyComponent::Direction::Left:
				pepper->GetTransform()->SetLocalRotation({ glm::vec3 {180.0f ,0 ,0} });
				pepper->GetTransform()->SetLocalPosition({ glm::vec3 {0 ,32 ,0} });
				break;
				case RigidbodyComponent::Direction::Right:
					pepper->GetTransform()->SetLocalPosition({ glm::vec3 {32 ,0 ,0} });

				break;
			}

		}

		m_PlayerComponent->SetIsShooting(true);
		m_PlayerComponent->SetPepper(pepper.get());
	}
}
