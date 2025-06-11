#pragma once
#include "Component.h"

namespace dae
{
	class IngredientPartComponent :public Component
	{
	public:
		explicit IngredientPartComponent(dae::GameObject& owner, const float& activationFalldown);

		void Update(float deltaTime) override;
		void FixedUpdate() override;
		void Render(glm::vec3 position, glm::vec2 scale) override;
		void PostUpdate(float deltaTime) override;

		void Activate();
		void Deactivate();
		bool Activated() const { return m_Activated; }

		void SetActivatable(const bool& activatable) { m_CanBeActivated = activatable;  }

	private:
		const float ACTIVATION_FALLDOWN;
		bool m_Activated = false;
		bool m_CanBeActivated = true;
	};
}



