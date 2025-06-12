#include "ClimbingEnemyState.h"

#include "Commands.h"
#include "Controller.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "VulnerableState.h"
#include "WalkingEnemyState.h"

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

		const float speed{ 500.f };
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

		//if (event.id == make_sdbm_hash("collision_enter"))
		//{
		//	auto* enemyComponent = object.GetComponent<EnemyComponent>();
		//	enemyComponent->SetState(std::make_unique<VulnerableState>());
		//}
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

		if (m_MapWalker->IsNextAvailable((directionToEnemy.x > 0.0f)))
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

