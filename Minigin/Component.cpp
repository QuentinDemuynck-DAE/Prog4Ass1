#include "Component.h"

Component::Component(dae::GameObject& owner)
	: m_owner(owner)
{
}

void Component::Update(float)
{
}

void Component::Render(glm::vec3, glm::vec2)
{
}

void Component::PostUpdate(float)
{
}
