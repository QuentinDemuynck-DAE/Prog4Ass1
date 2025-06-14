#pragma once
#include "Command.h"

#include <iostream>
#include <string>

#include "Observer.h"

namespace dae
{
	class MenuController;
	enum class GameMode;
}

namespace dae
{
	class MapWalkerComponent;
}

class PlayerComponent;

class HealthComponent;
class ScoreComponent;

namespace dae
{
	class RigidbodyComponent;
	class LogCommand : public Command
	{
	public:
		LogCommand(const std::string& message) : m_Message{ message } {}
		void Execute() override { std::cout << m_Message << std::endl; }
	
	private:
		std::string m_Message;
	};
	
	class MoveTransformCommand : public Command
	{
	public:
		MoveTransformCommand(dae::GameObject* gameObject, float x, float y);
		void Execute() override;
	
	private:
		float m_X;
		float m_Y;
		dae::GameObject* m_GameObject;
		MapWalkerComponent* m_pMapWalker;
		RigidbodyComponent* m_pRigidbody;
	};
	
	class DamageCommand : public Command
	{
	public:
		DamageCommand(dae::GameObject* gameObject, int amount = 10);
		void Execute() override;
	
	private:
		dae::GameObject* m_GameObject;
		HealthComponent* m_HealthComponent;
		int m_Amount;
	
	};

	class GetOffLadderCommand : public Command
	{
	public:
		GetOffLadderCommand(dae::GameObject* gameObject);
		void Execute() override;
	private:
		dae::GameObject* m_GameObject;
		MapWalkerComponent* m_MapWalkerComponent;
	};

	class GetOnLadderCommand : public Command
	{
	public:
		GetOnLadderCommand(dae::GameObject* gameObject);
		void Execute() override;
	private:
		dae::GameObject* m_GameObject;
		MapWalkerComponent* m_MapWalkerComponent;
	};

	class MenuUpCommand : public Command
	{
	public:
		MenuUpCommand(MenuController* menu) : m_Menu(menu) {}
		void Execute() override;
	private:
		MenuController* m_Menu;
	};

	class MenuDownCommand : public Command
	{
	public:
		MenuDownCommand(MenuController* menu) : m_Menu(menu) {}
		void Execute() override;
	private:
		MenuController* m_Menu;
	};

	class MenuClickCommand : public Command
	{
	public:
		MenuClickCommand(MenuController* menu) : m_Menu(menu) {}
		void Execute() override;
	private:
		MenuController* m_Menu;
	};

	class LoadSceneCommand : public Command
	{
	public:
		LoadSceneCommand(GameMode gameMode);
		void Execute() override;
	private:
		GameMode m_GameMode;
	};

	class SkipLevelCommand : public Command
	{
	public:
		SkipLevelCommand();
		void Execute() override;
	private:
	};

	class MuteSoundCommand : public Command
	{
	public:
		MuteSoundCommand();
		void Execute() override;
	private:
	};
}