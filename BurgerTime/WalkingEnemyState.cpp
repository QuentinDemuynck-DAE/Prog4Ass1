#include "WalkingEnemyState.h"

#include "ClimbingEnemyState.h"
#include "Commands.h"
#include "Controller.h"
#include "EnemyComponent.h"
#include "Globals.h"
#include "VulnerableState.h"
#include "GameObject.h"
#include "MapWalkerComponent.h"
#include <cmath>


void dae::WalkingEnemyState::OnEnter(dae::GameObject& gameObject)
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

	const float speed{ 35.f };
	if (m_EnemyController)
	{
		m_EnemyController->Bind(dae::Action::Up, std::make_shared<dae::GetOnLadderCommand>(&gameObject), KeyState::Down);
		m_EnemyController->Bind(dae::Action::Down, std::make_shared<dae::GetOnLadderCommand>(&gameObject), KeyState::Down);
		m_EnemyController->Bind(dae::Action::Left, std::make_shared<dae::MoveTransformCommand>(&gameObject, -speed, 0.f), KeyState::Pressed);
		m_EnemyController->Bind(dae::Action::Right, std::make_shared<dae::MoveTransformCommand>(&gameObject, speed, 0.f), KeyState::Pressed);
	}

	glm::vec3 direction = m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition() - gameObject.GetTransform()->GetGlobalPosition();

	if (glm::length(direction) > 0.0f)
		m_InitialDirection = glm::normalize(direction).x;
	else
		m_InitialDirection = 1.0f;

	m_GoalClimbDirection = direction.y > 0.0f
		 ? MapWalkerComponent::ClimbDirection::DOWN
		 : MapWalkerComponent::ClimbDirection::UP;
}

void dae::WalkingEnemyState::HandleInput(dae::GameObject& object, const Event& event)
{

	if (event.id == make_sdbm_hash("get_on_ladder"))
	{
		if (event.numberArgs > 0 && std::holds_alternative<glm::vec3>(event.args[0]))
		{
			glm::vec3 snapPos1 = std::get<glm::vec3>(event.args[0]);
			object.GetTransform()->SetLocalPosition(snapPos1);
		}

		if (m_EnemyComponent)
			m_EnemyComponent->SetState(std::make_unique<ClimbingEnemyState>());
	}

	if ( event.id == make_sdbm_hash("out_of_bounds"))
	{
		if (m_Timer > MIN_WALK_TIME)
			m_EnemyController->PerformAction(Action::Up);

		if (m_TimeSinceLastWallHit > WALL_HIT_GRACE)
		{
			m_InitialDirection = -m_InitialDirection;
			m_TimeSinceLastWallHit = 0.0f;
		}
	}

	//if (event.id == make_sdbm_hash("collision_enter"))
	//{
	//	auto* enemyComponent = object.GetComponent<EnemyComponent>();
	//	enemyComponent->SetState(std::make_unique<VulnerableState>());
	//}
}

void dae::WalkingEnemyState::Update(dae::GameObject& gameObject, float deltaTime)
{
	if (!m_EnemyController || !m_MapWalker)
		return;

	m_Timer += deltaTime;
	m_TimeSinceLastWallHit += deltaTime;

	UpdateGoalDirection(gameObject);

	if (m_InitialDirection > 0.0f)
		m_EnemyController->PerformAction(Action::Right);
	else
		m_EnemyController->PerformAction(Action::Left);


	float heightDiff = std::abs(m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition().y
		- gameObject.GetTransform()->GetGlobalPosition().y);

	bool passedPlayer =
		(m_InitialDirection > 0.0f && gameObject.GetTransform()->GetGlobalPosition().x > m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition().x) ||
		(m_InitialDirection < 0.0f && gameObject.GetTransform()->GetGlobalPosition().x < m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition().x);

	if (m_Timer >= MIN_WALK_TIME && m_GoalClimbDirection != MapWalkerComponent::ClimbDirection::NONE)
	{
		if (heightDiff > 6.0f)
			//&& (climbLadder.possibleDirections == MapWalkerComponent::ClimbDirection::BOTH || m_GoalClimbDirection == climbLadder.possibleDirections)) //If you can climb in the direction you want to go do so
		{
			
			m_EnemyController->PerformAction(Action::Up);
		}
	}
	else if (m_GoalClimbDirection == MapWalkerComponent::ClimbDirection::NONE && passedPlayer ) // passed player on same layer
	{
		m_EnemyController->PerformAction(Action::Up);
	}

}


void dae::WalkingEnemyState::OnExit(GameObject&)
{
	if (m_EnemyController)
	{
		m_EnemyController->Unbind(dae::Action::Up);
		m_EnemyController->Unbind(dae::Action::Down);
		m_EnemyController->Unbind(dae::Action::Left);
		m_EnemyController->Unbind(dae::Action::Right);
	}
}

void dae::WalkingEnemyState::UpdateGoalDirection(GameObject& gameObject)
{
	glm::vec3 direction = m_EnemyComponent->GetClosestPlayer()->GetTransform()->GetGlobalPosition() - gameObject.GetTransform()->GetGlobalPosition();
	const float differentFloorValue{ 6.0f };

	if (std::abs(direction.y) < differentFloorValue) // is on the same floor
	{
		m_GoalClimbDirection = MapWalkerComponent::ClimbDirection::NONE;
		return;
	}

	m_GoalClimbDirection = direction.y > 0.0f
		? MapWalkerComponent::ClimbDirection::DOWN
		: MapWalkerComponent::ClimbDirection::UP;
}
