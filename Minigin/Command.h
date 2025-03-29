#pragma once
#include <iostream>
#include <string>
class RigidbodyComponent;
class HealthComponent;
class ScoreComponent;

namespace dae
{
	class GameObject;

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};


	class GameActorCommand : public Command
	{
	public:
		GameActorCommand(class GameActor* pActor) : m_pActor{ pActor } {}
	protected:
		class GameActor* GetActor() const { return m_pActor; }
	private:
		class GameActor* m_pActor;
	};

}


