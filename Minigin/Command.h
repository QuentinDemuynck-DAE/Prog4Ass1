#pragma once
#include <iostream>
#include <string>
#include "GameObject.h"

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

class LogCommand : public Command
{
public:
	LogCommand(const std::string& message) : m_Message{ message } {}
	void Execute() override { std::cout << m_Message << std::endl;  }

private:
	std::string m_Message;
};

class MoveTransformCommand : public Command
{
public:
	MoveTransformCommand(dae::GameObject* gameObject, float x, float y) : m_GameObject{ gameObject }, m_X{ x }, m_Y{ y } {}
	void Execute() override
	{
		if(!m_GameObject)
			return;

		auto pos = m_GameObject->GetTransform()->GetLocalPosition();
		m_GameObject->GetTransform()->SetLocalPosition(pos.x + m_X, pos.y + m_Y, pos.z);
	};

private:
	float m_X, m_Y;
	dae::GameObject* m_GameObject;
};