#include "IngredientComponent.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "IngredientPartComponent.h"
#include "IngredientPartObserver.h"
#include "LandedState.h"
#include "Minigin.h"
#include "Subject.h"
#include "Texture2D.h"
#include "Texture2DComponent.h"

namespace dae
{
	IngredientComponent::IngredientComponent(dae::GameObject& owner, glm::ivec4 srcRect) : Component(owner), m_TextureSourceRectsStarter(srcRect)
	{
		m_pIngredientPartObjects.clear();
		for (int i{} ; i < NUM_CHILDS; i++)
		{
			int newColumnIdx = m_TextureSourceRectsStarter.x + i;

			glm::vec3 position
			{ (m_TextureSourceRectsStarter.z * i * GetOwner().GetTransform()->GetGlobalScale().x)
				, 0
				,0 };

			glm::ivec4 newSrcRect;
			newSrcRect.x = newColumnIdx * m_TextureSourceRectsStarter.z;
			newSrcRect.y = m_TextureSourceRectsStarter.y * m_TextureSourceRectsStarter.w;
			newSrcRect.z = m_TextureSourceRectsStarter.z;
			newSrcRect.w = m_TextureSourceRectsStarter.w;

			auto observer = std::make_shared<IngredientPartObserver>(&GetOwner());

			auto child = std::make_shared<GameObject>(position);
			child->AddComponent<Texture2DComponent>("allAssets.png", newSrcRect);
			child->AddComponent<CollisionComponent>(*dae::Minigin::physicsWorld.get(), CollisionLayers::INGREDIENT ,CollisionLayers::PLAYER, glm::vec2{ m_TextureSourceRectsStarter.z -1, m_TextureSourceRectsStarter.w -1 }, glm::vec2{ 1, 1 }, true, false);
			child->AddComponent<IngredientPartComponent>(ACTIVATION_FALLDOWN);
			child->SetParent(&GetOwner());
			child->GetSubject()->AddObserver(observer);
			m_pIngredientPartObjects.push_back(child);
		}

	}


	void IngredientComponent::HandleInput(const Event event)
	{
		if (m_pState) m_pState->HandleInput(GetOwner(), event);
	}

	void IngredientComponent::SetState(std::unique_ptr<IngredientState> newState)
	{
		if (m_pState) m_pState->OnExit(GetOwner());
		m_pState = std::move(newState);
		if (m_pState) m_pState->OnEnter(GetOwner());
	}

	void IngredientComponent::DeactivateAllChilds()
	{
		std::for_each(m_pIngredientPartObjects.begin(), m_pIngredientPartObjects.end(),
			[&](std::shared_ptr<GameObject> child) 
			{child.get()->GetComponent<IngredientPartComponent>()->Deactivate(); });
	}

	void IngredientComponent::SetActivatableAllPArts(const bool& activatable)
	{
		std::for_each(m_pIngredientPartObjects.begin(), m_pIngredientPartObjects.end(),
			[&](std::shared_ptr<GameObject> child)
			{child.get()->GetComponent<IngredientPartComponent>()->SetActivatable(activatable); });
	}

	void IngredientComponent::Update(float deltaTime)
	{
		Component::Update(deltaTime);

		if (m_pState) 
			m_pState->Update(GetOwner(), deltaTime);
		else
		{
			auto state = std::make_unique<LandedState>();
			SetState(std::move(state));
		}

	}
	
}

