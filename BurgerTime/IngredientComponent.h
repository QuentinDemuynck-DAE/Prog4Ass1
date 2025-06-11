#pragma once
#include <memory>
#include <vector>

#include "Component.h"

namespace dae
{
	class GameObject;
	class IngredientComponent final : public Component
	{
	public:
		typedef std::vector<std::unique_ptr<GameObject>> ChildObjectsPtrs;

		explicit IngredientComponent(dae::GameObject& owner);
		void Update(float deltaTime) override;
		void FixedUpdate() override;
		void Render(glm::vec3 position, glm::vec2 scale) override;
		void PostUpdate(float deltaTime) override;
	private:

		ChildObjectsPtrs m_pIngredientPartObjects;
	};
}


