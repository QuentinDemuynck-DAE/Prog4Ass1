#pragma once
#include <memory>
#include <vector>

#include "Component.h"
#include "IngredientState.h"

namespace dae
{
	class GameObject;
	class IngredientComponent final : public Component
	{
	public:
		typedef std::vector<std::shared_ptr<GameObject>> ChildObjectsPtrs;

		explicit IngredientComponent(dae::GameObject& owner, glm::ivec4 srcRect);
		void Update(float deltaTime) override;


		void HandleInput(const Event event);
		void SetState(std::unique_ptr<IngredientState> newState);

	private:
		const int NUM_CHILDS = 4;
		ChildObjectsPtrs m_pIngredientPartObjects;
		glm::ivec4 m_TextureSourceRectsStarter;
		std::unique_ptr<IngredientState> m_pState;
	};
}


