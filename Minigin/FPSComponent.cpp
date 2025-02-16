#include "FPSComponent.h"

void FPSComponent::Update(float deltaTime)
{
	m_pTextComponent->Update(deltaTime);

	if (deltaTime > 0.0f)
		m_FPS = 1.0f / deltaTime;

	if (m_counter >= 15)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << m_FPS;
		m_pTextComponent->SetText(ss.str());

		m_counter = 0;
	}
	
}

void FPSComponent::Render(glm::vec3 position) const
{
	if (m_showFPS)
		m_pTextComponent->Render(position);
}

void FPSComponent::FixedUpdate()
{
	m_counter++;
}

FPSComponent::FPSComponent(std::shared_ptr<dae::Font> font)
	: m_FPS{ 0 }
{
	m_pTextComponent = std::make_unique<dae::TextComponent>("60.0", font);
}
