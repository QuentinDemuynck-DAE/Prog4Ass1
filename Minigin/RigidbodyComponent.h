#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae 
{
	class GameObject;
	class RigidbodyComponent : public Component
	{



	public:

		enum class Direction {
			None,
			Up,
			Down,
			Left,
			Right
		};

		RigidbodyComponent(dae::GameObject& owner, float maxSpeed = 10.f, float mass = 10.f, float drag = 0.0f);
		~RigidbodyComponent() = default;
		void Update(float deltaTime) override;
		void AddVelocity(float x, float y);
		void AddVelocity(glm::vec2 velocity);
		void SetDrag(float drag);
		glm::vec2 GetVelocity() const;
		float GetDrag() const;
		glm::vec2 GetLastAddedVelocity() const;

		void Stop();
		void Freeze();
		void Resume();

		static Direction GetCardinalDirection(const glm::vec2& vel, float deadZone = 0.001f) 
		{
			if (glm::length(vel) < deadZone)
				return Direction::None;

			float absX = std::abs(vel.x);
			float absY = std::abs(vel.y);

			if (absX > absY) {
				// Horizontal is dominant
				return (vel.x > 0) ? Direction::Right : Direction::Left;
			}
			else {
				// Vertical is dominant
				return (vel.y > 0) ? Direction::Up : Direction::Down;
			}
		}

	private:
		float m_Mass;
		float m_Drag;
		glm::vec2 m_Velocity{ 0.f,0.f };
		float m_MaxSpeed{ 10.f };
		glm::vec2 m_LastAddedVelocity{ 0.f,0.f };
		bool m_Freezed{ false };

	};
}


