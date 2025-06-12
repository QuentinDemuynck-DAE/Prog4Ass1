#include "Walking.h"

#include "Climbing.h"
#include "Commands.h"
#include "GameObject.h"
#include "GamePad.h"
#include "MapWalkerComponent.h"
#include "PlayerComponent.h"
#include "RigidbodyComponent.h"
#include <glm.hpp>

#include "Controller.h"

void dae::Walking::OnEnter(dae::GameObject& game_object)
{
	const float speed{ 100.f };

	if (game_object.HasComponent<PlayerComponent>())
		m_PlayerComponent = game_object.GetComponent<PlayerComponent>();

	if (game_object.HasComponent<RigidbodyComponent>())
		m_Rigidbody = game_object.GetComponent<RigidbodyComponent>();

	if (game_object.HasComponent<MapWalkerComponent>())
		m_MapWalker = game_object.GetComponent<MapWalkerComponent>();

	if (game_object.HasComponentDerived<ControllerComponent>())
		m_PlayerController = game_object.GetComponentDerived<ControllerComponent>();

	if (m_PlayerController)
	{
		m_PlayerController->Bind(dae::Action::Up, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);
		m_PlayerController->Bind(dae::Action::Down, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);
		m_PlayerController->Bind(dae::Action::Left, std::make_shared<dae::MoveTransformCommand>(&game_object, -speed, 0.f), KeyState::Pressed);
		m_PlayerController->Bind(dae::Action::Right, std::make_shared<dae::MoveTransformCommand>(&game_object, speed, 0.f), KeyState::Pressed);
	}

}

void dae::Walking::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("get_on_ladder"))
	{
		if (event.numberArgs > 0 && std::holds_alternative<glm::vec3>(event.args[0]))
		{
			glm::vec3 snapPos1 = std::get<glm::vec3>(event.args[0]);
			object.GetTransform()->SetLocalPosition(snapPos1);
		}

		auto* enemyComponent = object.GetComponent<PlayerComponent>();
		enemyComponent->SetState(std::make_unique<Climbing>());
	}

	if (event.id == make_sdbm_hash("controller_added"))
	{
		auto* enemyComponent = object.GetComponent<PlayerComponent>();
		enemyComponent->SetState(std::make_unique<Walking>());
	}
}

void dae::Walking::Update(GameObject&, float)
{

}

void dae::Walking::OnExit(GameObject&)
{
	if (m_PlayerController)
	{
		m_PlayerController->Unbind(dae::Action::Up);
		m_PlayerController->Unbind(dae::Action::Down);
		m_PlayerController->Unbind(dae::Action::Left);
		m_PlayerController->Unbind(dae::Action::Right);
	}
}
