#include "Texture2DComponent.h"

void Texture2DComponent::Render(glm::vec3 position, glm::vec2 scale)
{
	dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y, scale, m_SourceRect);
}

void Texture2DComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

Texture2DComponent::Texture2DComponent(dae::GameObject& owner, const std::string& filename)
	:Component(owner)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

Texture2DComponent::Texture2DComponent(dae::GameObject& owner, const std::string& filename, glm::ivec4 sourceRect)
	:Texture2DComponent(owner, filename)
{
	m_SourceRect = sourceRect;
}

