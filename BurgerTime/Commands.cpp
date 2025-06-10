#include "Commands.h"
#include "GameObject.h"
#include "Globals.h"
#include "RigidbodyComponent.h"
#include "HealthComponent.h"
#include "MapWalkerComponent.h"
#include "ScoreComponent.h"
#include "Subject.h"


dae::MoveTransformCommand::MoveTransformCommand(dae::GameObject* gameObject, float x, float y)
	: m_GameObject(gameObject), m_X(x), m_Y(y), m_pRigidbody(nullptr)
{
	if (m_GameObject && m_GameObject->HasComponent<RigidbodyComponent>()) {
		m_pRigidbody = m_GameObject->GetComponent<RigidbodyComponent>();
	}
}


void dae::MoveTransformCommand::Execute()
{
	if (!m_GameObject)
		return;

	if (m_pRigidbody)
		m_pRigidbody->AddVelocity(m_X, m_Y);
};

dae::DamageCommand::DamageCommand(dae::GameObject* gameObject, int amount)
	:m_GameObject(gameObject), m_Amount(amount)
{
	if (m_GameObject->HasComponent<HealthComponent>())
	{
		m_HealthComponent = m_GameObject->GetComponent<HealthComponent>();
	}
}

void dae::DamageCommand::Execute()
{
	if (m_GameObject && m_HealthComponent)
	{
		m_HealthComponent->DoDamage();
	}
}

dae::ScoreCommand::ScoreCommand(dae::GameObject* gameObject, int amount)
	:m_GameObject(gameObject), m_Amount(amount)
{
	if (m_GameObject->HasComponent<ScoreComponent>())
	{
		m_ScoreComponent = m_GameObject->GetComponent<ScoreComponent>();
	}
}

void dae::ScoreCommand::Execute()
{
	if (m_ScoreComponent)
	{
		m_ScoreComponent->AddScore(m_Amount);
	}
}

dae::GetOffLadderCommand::GetOffLadderCommand(dae::GameObject* gameObject)
	:m_GameObject(gameObject)
{
	if (m_GameObject->HasComponent<MapWalkerComponent>())
	{
		m_MapWalkerComponent = m_GameObject->GetComponent<MapWalkerComponent>();
	}
}

void dae::GetOffLadderCommand::Execute()
{
	if (!m_MapWalkerComponent)
		return;

	auto query = m_MapWalkerComponent->QueryExitLadder();
	if (query.canPerform)
	{
		Event event{ make_sdbm_hash("get_off_ladder") };
		event.AddArg(query.snapPosition);
		m_GameObject->GetSubject()->Notify(event);
	}
}

dae::GetOnLadderCommand::GetOnLadderCommand(dae::GameObject* gameObject)
	:m_GameObject(gameObject)
{
	if (m_GameObject->HasComponent<MapWalkerComponent>())
	{
		m_MapWalkerComponent = m_GameObject->GetComponent<MapWalkerComponent>();
	}
}

void dae::GetOnLadderCommand::Execute()
{
	if (!m_MapWalkerComponent)
		return;

	auto query = m_MapWalkerComponent->QueryClimbLadder();
	if (query.canPerform)
	{
		Event event{ make_sdbm_hash("get_on_ladder") };
		event.AddArg(query.snapPosition);
		m_GameObject->GetSubject()->Notify(event);
	}
}
