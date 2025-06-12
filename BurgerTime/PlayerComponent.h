#pragma once
#include <memory>

#include "Component.h"
#include "PlayerState.h"

struct Event;

namespace dae
{
	class GamePad;
}

class PlayerComponent final: public Component
{
public:
	PlayerComponent(dae::GameObject& owner);
	virtual ~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

	void Update(float deltaTime) override;

	void HandleInput(const Event event);

	void SetState(std::unique_ptr<dae::PlayerState> newState);
	void ResetShootingTimer() { m_CurrentlyShooting = 0.0f; }
	void SetIsShooting(bool isShooting)
	{
		m_IsShooting = isShooting;
	}
	bool GetIsShooting() const { return m_IsShooting; }
	float GetShootingTime() const  { return m_CurrentlyShooting; }
	void SetPepper(dae::GameObject* pepper) { m_Pepper = pepper; }
	float GetPepperCooldown() const { return m_timeSinceLastPepper; }
	void ResetPepperCooldown() { m_timeSinceLastPepper = 0; }
	void StopShooting();


	const float PEPPER_COOLDOWN = 5.0f;
	const float MAX_PEPPER_TIME = 2.0f;

private:
	std::unique_ptr<dae::PlayerState>  m_CurrentState;
	bool m_IsShooting = false;
	float m_CurrentlyShooting = 0.0f;
	dae::GameObject* m_Pepper = nullptr;
	float m_timeSinceLastPepper;
};

