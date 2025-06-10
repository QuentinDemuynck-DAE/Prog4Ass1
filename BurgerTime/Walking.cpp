#include "Walking.h"

#include "Climbing.h"
#include "Commands.h"
#include "GameObject.h"
#include "GamePad.h"
#include "MapWalkerComponent.h"
#include "PlayerComponent.h"
#include "RigidbodyComponent.h"
#include <glm.hpp>

void dae::Walking::OnEnter(dae::GameObject& game_object)
{
	const float speed{ 100.f };

	if (game_object.HasComponent<PlayerComponent>())
		m_PlayerComponent = game_object.GetComponent<PlayerComponent>();

	if (game_object.HasComponent<RigidbodyComponent>())
		m_Rigidbody = game_object.GetComponent<RigidbodyComponent>();

	if (game_object.HasComponent<MapWalkerComponent>())
		m_MapWalker = game_object.GetComponent<MapWalkerComponent>();

	if (m_PlayerComponent && m_PlayerComponent->GamePad())
	{
		m_PlayerComponent->GamePad()->BindCommand(dae::GamePad::Button::DPadUp, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);
		m_PlayerComponent->GamePad()->BindCommand(dae::GamePad::Button::DPadDown, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);

		m_PlayerComponent->GamePad()->BindCommand(dae::GamePad::Button::DPadLeft, std::make_shared<dae::MoveTransformCommand>(&game_object, -speed, 0.f), KeyState::Pressed);
		m_PlayerComponent->GamePad()->BindCommand(dae::GamePad::Button::DPadRight, std::make_shared<dae::MoveTransformCommand>(&game_object, speed, 0.f), KeyState::Pressed);
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
}

void dae::Walking::Update(GameObject& game_object, float x)
{
	PlayerState::Update(game_object, x);
	if (!m_MapWalker)
		return;

	if (!m_MapWalker->IsOnFloor())
	{

		m_Rigidbody->AddVelocity(0.0f, 2.f);
	}

}

void dae::Walking::OnExit(GameObject&)
{
	if (m_PlayerComponent && m_PlayerComponent->GamePad())
	{

		m_PlayerComponent->GamePad()->UnbindCommand(dae::GamePad::Button::DPadUp);
		m_PlayerComponent->GamePad()->UnbindCommand(dae::GamePad::Button::DPadDown);
		m_PlayerComponent->GamePad()->UnbindCommand(dae::GamePad::Button::DPadLeft);
		m_PlayerComponent->GamePad()->UnbindCommand(dae::GamePad::Button::DPadRight);
	}
}
