#include "Walking.h"

#include "Climbing.h"
#include "Commands.h"
#include "GameObject.h"
#include "GamePad.h"
#include "MapWalkerComponent.h"
#include "PlayerComponent.h"
#include "RigidbodyComponent.h"
#include <glm.hpp>
#include "CollisionComponent.h"

#include "Controller.h"
#include "Dying.h"
#include "EnemyComponent.h"

void dae::Walking::OnEnter(dae::GameObject& game_object)
{
	const float speed{ 500.f };

	if (game_object.HasComponent<PlayerComponent>())
		m_PlayerComponent = game_object.GetComponent<PlayerComponent>();

	if (game_object.HasComponent<RigidbodyComponent>())
		m_Rigidbody = game_object.GetComponent<dae::RigidbodyComponent>();

	if (game_object.HasComponent<MapWalkerComponent>())
		m_MapWalker = game_object.GetComponent<MapWalkerComponent>();

	if (game_object.HasComponentDerived<ControllerComponent>())
		m_PlayerController = game_object.GetComponentsDerived<ControllerComponent>();

	for (auto controller : m_PlayerController)
	{
		if (controller)
		{
			controller->Bind(dae::Action::Up, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);
			controller->Bind(dae::Action::Down, std::make_shared<dae::GetOnLadderCommand>(&game_object), KeyState::Down);
			controller->Bind(dae::Action::Left, std::make_shared<dae::MoveTransformCommand>(&game_object, -speed, 0.f), KeyState::Pressed);
			controller->Bind(dae::Action::Right, std::make_shared<dae::MoveTransformCommand>(&game_object, speed, 0.f), KeyState::Pressed);
		}
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

		auto* playerComponent = object.GetComponent<PlayerComponent>();
		playerComponent->SetState(std::make_unique<Climbing>());
	}

	if (event.id == make_sdbm_hash("controller_added"))
	{
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

			if (receiver->GetOwner().HasComponent<EnemyComponent>() && receiver->GetOwner().GetComponent<EnemyComponent>()->CanHit()) //Collided with a falling ingredient so also fall down
			{
				auto state = std::make_unique<Dying>();
				m_PlayerComponent->SetState(std::move(state));
			}

		}
	}
}

void dae::Walking::Update(GameObject&, float)
{

}

void dae::Walking::OnExit(GameObject&)
{
	for (auto controller : m_PlayerController)
	{
		if (controller)
		{
			controller->Unbind(dae::Action::Up);
			controller->Unbind(dae::Action::Down);
			controller->Unbind(dae::Action::Left);
			controller->Unbind(dae::Action::Right);
		}
	}
}
