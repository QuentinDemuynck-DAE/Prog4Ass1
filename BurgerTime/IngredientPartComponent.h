#pragma once
#include "Component.h"

namespace dae
{
	class IngredientPartComponent : Component
	{
	public:
		explicit IngredientPartComponent(dae::GameObject& owner);

		void Update(float deltaTime) override;
		void FixedUpdate() override;
		void Render(glm::vec3 position, glm::vec2 scale) override;
		void PostUpdate(float deltaTime) override;

		void Activate();
		void Deactivate();

	private:
		bool m_Activated;
	};
}



