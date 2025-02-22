#include "FPSComponent.h"

void FPSComponent::Update(float deltaTime)
{
	if (deltaTime > 0.0f)
		m_FPS = 1.0f / deltaTime;

	if (m_counter >= 15)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << m_FPS;

		if (m_textComponent)
			m_textComponent->SetText(ss.str());

		m_counter = 0;
	}

	// If the text component is removed, remove this component as well
	if (!GetOwner().HasComponent<dae::TextComponent>()) 
	{
		Destroy();
	}
}

void FPSComponent::FixedUpdate()
{
	m_counter++;
}

FPSComponent::FPSComponent(dae::GameObject& owner)
	:Component(owner), m_FPS{ 0 }
{
	if (owner.HasComponent<dae::TextComponent>())
	{
		m_textComponent = owner.GetComponent<dae::TextComponent>();
	}
}

