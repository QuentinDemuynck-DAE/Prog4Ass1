#include "IngredientComponent.h"
#include "GameObject.h"
namespace dae
{
	IngredientComponent::IngredientComponent(dae::GameObject& owner) : Component(owner)
	{
	}

	void IngredientComponent::Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}
	
	void IngredientComponent::FixedUpdate()
	{
		Component::FixedUpdate();
	}
	
	void IngredientComponent::Render(glm::vec3 position, glm::vec2 scale)
	{
		Component::Render(position, scale);
	}
	
	void IngredientComponent::PostUpdate(float deltaTime)
	{
		Component::PostUpdate(deltaTime);
	}
}

