#include "IngredientPartComponent.h"
#include "GameObject.h"


namespace dae
{
	IngredientPartComponent::IngredientPartComponent(dae::GameObject& owner) : Component(owner)
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
}

