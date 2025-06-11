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

		void DeactivateAllChilds();
		void SetActivatableAllPArts(const bool& activatable);
		const int GetNumChilds() const { return NUM_CHILDS; }
		const float GetActivationFalldown() const { return ACTIVATION_FALLDOWN; }


	private:
		const int NUM_CHILDS = 4;
		const float ACTIVATION_FALLDOWN = 8.0f;
		ChildObjectsPtrs m_pIngredientPartObjects;
		glm::ivec4 m_TextureSourceRectsStarter;
		std::unique_ptr<IngredientState> m_pState;


		int m_ActivatedParts{ 0 };
	};
}


