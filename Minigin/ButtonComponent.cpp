#include "ButtonComponent.h"

#include "GameObject.h"
#include "TextComponent.h"

dae::ButtonComponent::ButtonComponent(GameObject& owner, std::unique_ptr<Command> command)
	:Component(owner), m_Command(std::move(command))
{
	if (owner.HasComponent<TextComponent>())
	{
		m_TextComponent = owner.GetComponent<TextComponent>();
	}
}

void dae::ButtonComponent::Click()
{
	m_Command->Execute();
}

void dae::ButtonComponent::SetSelected(bool selected)
{
	m_Selected = selected;

	if (m_Selected && m_TextComponent)
	{
		m_TextComponent->SetColor(TextComponent::Color{0,0,255});
	}
	else
	{
		m_TextComponent->SetColor(TextComponent::Color{ 255,255,255 });

	}
}
