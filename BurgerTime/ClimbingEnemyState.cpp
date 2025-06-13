#include "ClimbingEnemyState.h"

#include "Commands.h"
#include "Controller.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "PepperComponent.h"
#include "StunnedState.h"
#include "WalkingEnemyState.h"
#include "CollisionComponent.h"
#include "EnemyDying.h"
#include "EnemyFallingAlong.h"

namespace dae
{
	void ClimbingEnemyState::OnEnter(dae::GameObject& gameObject)
	{

		if (gameObject.HasComponent<EnemyComponent>())
		{
			m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
			m_Players = m_EnemyComponent->GetPlayers();
		}

		if (gameObject.HasComponentDerived<ControllerComponent>())
			m_EnemyController = gameObject.GetComponentDerived<ControllerComponent>();

		if (gameObject.HasComponent<MapWalkerComponent>())
		{
			m_MapWalker = gameObject.GetComponent<MapWalkerComponent>();
		}

		const float speed{ 25.f };
		if (m_EnemyController)
		{
			m_EnemyController->Bind(dae::Action::Up, std::make_shared<dae::MoveTransformCommand>(&gameObject, 0.0f, -speed), KeyState::Pressed);
			m_EnemyController->Bind(dae::Action::Down, std::make_shared<dae::MoveTransformCommand>(&gameObject, 0.0f, speed), KeyState::Pressed);
			m_EnemyController->Bind(dae::Action::Left, std::make_shared<dae::GetOffLadderCommand>(&gameObject), KeyState::Down);
			m_EnemyController->Bind(dae::Action::Right, std::make_shared<dae::GetOffLadderCommand>(&gameObject), KeyState::Down);
		}

		glm::vec3 direction = m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition() - gameObject.GetTransform()->GetGlobalPosition();

		if (glm::length(direction) > 0.0f)
			m_InitialDirection = glm::normalize(direction).y;
		else
			m_InitialDirection = 1.0f;

		m_ClimbDirection = direction.y > 0.0f ? MapWalkerComponent::ClimbDirection::DOWN : MapWalkerComponent::ClimbDirection::UP;
	}

	void ClimbingEnemyState::OnExit(dae::GameObject&)
	{
		if (m_EnemyController)
		{
			m_EnemyController->Unbind(dae::Action::Up);
			m_EnemyController->Unbind(dae::Action::Down);
			m_EnemyController->Unbind(dae::Action::Left);
			m_EnemyController->Unbind(dae::Action::Right);
		}
	}

	void ClimbingEnemyState::HandleInput(dae::GameObject& object, const Event& event)
	{
		// COLLISION ENTER
		if (event.id == make_sdbm_hash("collision_enter"))
		{
			// 0 = sender 1= other;
			if (event.numberArgs >= 2
				&& std::holds_alternative<void*>(event.args[0])
				&& std::holds_alternative<void*>(event.args[1]))
			{

				//Only check if we sent it
				auto sender = static_cast<CollisionComponent*>(
					std::get<void*>(event.args[0]));

				auto receiver = static_cast<CollisionComponent*>(
					std::get<void*>(event.args[1]));

				if (!(sender && receiver && sender == object.GetComponent<dae::CollisionComponent>()))
					return;

				if (receiver->GetOwner().HasComponent<PepperComponent>()) //Collided with a falling ingredient so also fall down
				{
					auto state = std::make_unique<StunnedState>(false);
					m_EnemyComponent->SetState(std::move(state));
				}

			}
		}


		if (event.id == make_sdbm_hash("ingredient_started_falling"))
		{
			if (event.numberArgs >= 1
				&& std::holds_alternative<void*>(event.args[0]))
			{
				auto sender = static_cast<GameObject*>(
					std::get<void*>(event.args[0]));

				auto state = std::make_unique<EnemyFallingAlong>(sender);
				m_EnemyComponent->SetState(std::move(state));
			}
		}

		if (event.id == make_sdbm_hash("ingredient_fell_on_enemy"))
		{
			if (event.numberArgs >= 1
				&& std::holds_alternative<void*>(event.args[0]))
			{

				auto state = std::make_unique<EnemyDying>();
				m_EnemyComponent->SetState(std::move(state));
			}
		}

		if (event.id == make_sdbm_hash("get_off_ladder"))
		{
			if (event.numberArgs > 0 && std::holds_alternative<glm::vec3>(event.args[0]))
			{
				glm::vec3 snapPos1 = std::get<glm::vec3>(event.args[0]);
				object.GetTransform()->SetLocalPosition(snapPos1);
			}

			auto* enemyComponent = object.GetComponent<EnemyComponent>();
			enemyComponent->SetState(std::make_unique<WalkingEnemyState>());
		}

	}

	void ClimbingEnemyState::Update(dae::GameObject& gameObject, float deltaTime)
	{

		switch (m_ClimbDirection)
		{
		case MapWalkerComponent::ClimbDirection::UP:
				m_EnemyController->PerformAction(Action::Up);
			break;

		case MapWalkerComponent::ClimbDirection::DOWN:

				m_EnemyController->PerformAction(Action::Down);
			break;
		}

		m_Timer += deltaTime;
		if (m_Timer < MIN_CLIMB_TIME)
			return;

		auto possibleClimbDirections = m_MapWalker->PossibleClimbDirections();

		auto directionToEnemy = m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition() - gameObject.GetTransform()->GetGlobalPosition();

		if (!m_MapWalker->IsNextAvailable((directionToEnemy.x > 0.0f)))
		{
			return;
		}


		if ((gameObject.GetTransform()->GetGlobalPosition().y < m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition().y && m_ClimbDirection == MapWalkerComponent::ClimbDirection::UP)
			|| (gameObject.GetTransform()->GetGlobalPosition().y > m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition().y && m_ClimbDirection == MapWalkerComponent::ClimbDirection::DOWN)
			|| (possibleClimbDirections == MapWalkerComponent::ClimbDirection::DOWN && m_ClimbDirection == MapWalkerComponent::ClimbDirection::UP)
			|| (possibleClimbDirections == MapWalkerComponent::ClimbDirection::UP && m_ClimbDirection == MapWalkerComponent::ClimbDirection::DOWN))
		{
			m_EnemyController->PerformAction(Action::Left);
		}


	}
}

