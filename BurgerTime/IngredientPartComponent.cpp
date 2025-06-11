#include "IngredientPartComponent.h"
#include "GameObject.h"


namespace dae
{
	IngredientPartComponent::IngredientPartComponent(dae::GameObject& owner, const float& activationFalldown) : Component(owner), ACTIVATION_FALLDOWN(activationFalldown)
	{
	}

	void IngredientPartComponent::Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}

	void IngredientPartComponent::FixedUpdate()
	{
		Component::FixedUpdate();
	}

	void IngredientPartComponent::Render(glm::vec3 position, glm::vec2 scale)
	{
		Component::Render(position, scale);
	}

	void IngredientPartComponent::PostUpdate(float deltaTime)
	{
		Component::PostUpdate(deltaTime);
	}

	void IngredientPartComponent::Activate()
	{
		if (!m_CanBeActivated)
			return;

		m_Activated = true;
		GetOwner().GetTransform()->SetLocalPosition({ GetOwner().GetTransform()->GetLocalPosition().x, ACTIVATION_FALLDOWN, GetOwner().GetTransform()->GetLocalPosition().z });
	}

	void IngredientPartComponent::Deactivate()
	{
		m_Activated = false;
		GetOwner().GetTransform()->SetLocalPosition({ GetOwner().GetTransform()->GetLocalPosition().x , 0.0f, GetOwner().GetTransform()->GetLocalPosition().z});
	}
}

