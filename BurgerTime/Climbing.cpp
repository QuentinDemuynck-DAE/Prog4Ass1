#include "Climbing.h"

#include "Commands.h"
#include "GameObject.h"
#include "GamePad.h"
#include "PlayerComponent.h"
#include "Walking.h"
#include <glm.hpp>

#include "CollisionComponent.h"
#include "Controller.h"
#include "Dying.h"
#include "EnemyComponent.h"

void dae::Climbing::OnEnter(dae::GameObject& game_object)
{
	const float speed{ 38.f };

	if (game_object.HasComponent<PlayerComponent>())
		m_PlayerComponent = game_object.GetComponent<PlayerComponent>();


	if (game_object.HasComponentDerived<ControllerComponent>())
		m_PlayerController = game_object.GetComponentDerived<ControllerComponent>();

	if (m_PlayerController)
	{
		m_PlayerController->Bind(dae::Action::Up, std::make_shared<dae::MoveTransformCommand>(&game_object, 0.0f, -speed), KeyState::Pressed);
		m_PlayerController->Bind(dae::Action::Down, std::make_shared<dae::MoveTransformCommand>(&game_object, 0.0f, speed), KeyState::Pressed);
		m_PlayerController->Bind(dae::Action::Left, std::make_shared<dae::GetOffLadderCommand>(&game_object), KeyState::Down);
		m_PlayerController->Bind(dae::Action::Right, std::make_shared<dae::GetOffLadderCommand>(&game_object), KeyState::Down);
	}

}

void dae::Climbing::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("get_off_ladder"))
	{
		if (event.numberArgs > 0 && std::holds_alternative<glm::vec3>(event.args[0]))
		{
			glm::vec3 snapPos1 = std::get<glm::vec3>(event.args[0]);
			object.GetTransform()->SetLocalPosition(snapPos1);
		}

		auto* playerComponent = object.GetComponent<PlayerComponent>();
		playerComponent->SetState(std::make_unique<Walking>());
	}

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

			if (receiver->GetOwner().HasComponent<EnemyComponent>()) //Collided with a falling ingredient so also fall down
			{
				auto state = std::make_unique<Dying>();
				m_PlayerComponent->SetState(std::move(state));
			}

		}
	}
}

void dae::Climbing::OnExit(GameObject&)
{
	if (m_PlayerController)
	{
		m_PlayerController->Unbind(dae::Action::Up);
		m_PlayerController->Unbind(dae::Action::Down);
		m_PlayerController->Unbind(dae::Action::Left);
		m_PlayerController->Unbind(dae::Action::Right);
	}
}