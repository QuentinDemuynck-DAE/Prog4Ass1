#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"

#include <algorithm>

#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(dae::GameObject& owner, const std::string& text, std::shared_ptr<Font> font)
	:Component(owner), m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{ 

}

void dae::TextComponent::Update(float)
{
	if (m_needsUpdate)
	{
		const SDL_Color color = {m_Color.r ,m_Color.g,m_Color.b,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render(glm::vec3 position, glm::vec2 scale)
{
	Component::Render(position, scale);
	if (m_textTexture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_textTexture, position.x, position.y, scale);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const Color& color)
{
	m_Color = color;
	m_needsUpdate = true;
}



