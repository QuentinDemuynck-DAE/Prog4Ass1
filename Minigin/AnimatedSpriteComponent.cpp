#include "AnimatedSpriteComponent.h"

#include <iostream>

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::AnimatedSpriteComponent::AnimatedSpriteComponent(dae::GameObject& owner, const std::string& filename,
	const int& startFrame, const int& endFrame, const glm::ivec2& imageSize, const glm::ivec2 gridSize, const float& frameDelay)
	:Component(owner),
	m_startFrame(startFrame),
	m_endFrame(endFrame),
	m_imageSize(imageSize),
	m_gridSize(gridSize),
	m_frameDelay(frameDelay),
	m_CurrentFrame(startFrame)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
	if (!(endFrame >= 0 && endFrame <= m_gridSize.x * m_gridSize.y && m_startFrame < endFrame))
	{
		m_endFrame = m_gridSize.x * m_gridSize.y;
		std::cout << "Endframe not in range , set to max" << std::endl;

	}
	if (!(startFrame >= 0 && startFrame < m_gridSize.x * m_gridSize.y && startFrame < m_endFrame))
	{
		m_startFrame = 0;
		std::cout << "Startframe not in range , set to 0" << std::endl;

	}
}

void dae::AnimatedSpriteComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);

}

void dae::AnimatedSpriteComponent::SetStartFrame(const int& startFrame)
{
	if (startFrame >= 0 && startFrame < m_gridSize.x * m_gridSize.y && startFrame < m_endFrame)
	{
		m_startFrame = startFrame;
		return;
	}

	std::cout << "Startframe not in range" << std::endl;
}

void dae::AnimatedSpriteComponent::SetEndFrame(const int& endFrame)
{
	if (endFrame >= 0 && endFrame <= m_gridSize.x * m_gridSize.y && m_startFrame < endFrame)
	{
		m_endFrame = endFrame;
		return;
	}

	std::cout << "Endframe must be larger than 0";
}

void dae::AnimatedSpriteComponent::SetColumnOrderedAnimation(const bool& colorOrderedAnimation)
{
	m_columnOrderedAnimation = colorOrderedAnimation;
}

void dae::AnimatedSpriteComponent::Loop(const bool& loop)
{
	m_loop = loop;
}

void dae::AnimatedSpriteComponent::Pause()
{
	m_isPlaying = false;
}

void dae::AnimatedSpriteComponent::Resume()
{
	m_isPlaying = true;
}

void dae::AnimatedSpriteComponent::Update(float deltaTime)
{
	if ((!m_loop && m_CurrentFrame == m_endFrame) || !m_isPlaying) return;

	m_Timer += deltaTime;
	if ( m_Timer >= m_frameDelay)
	{
		m_CurrentFrame += 1;

		m_Timer -= m_frameDelay;
	}

	if (m_CurrentFrame > m_endFrame)
	{
		m_CurrentFrame = m_startFrame;
	}
}

void dae::AnimatedSpriteComponent::Render(glm::vec3 position, glm::vec2 scale)
{
	glm::ivec4 srcRect = calculateSourceRect();

	float angleDeg = GetOwner().GetTransform()->GetLocalRotation().x;
	dae::Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y, scale, srcRect, angleDeg);
}

glm::ivec4 dae::AnimatedSpriteComponent::calculateSourceRect()
{
	int col = 0;
	int row = 0;

	// Prog 2 trick  :))
	if (m_columnOrderedAnimation)
	{
		col = (m_CurrentFrame / m_gridSize.x);
		row = (m_CurrentFrame % m_gridSize.x);
	}
	else
	{
		col = (m_CurrentFrame % m_gridSize.y);
		row = (m_CurrentFrame / m_gridSize.y);
	}

	const int srcX = col * m_imageSize.x;
	const int srcY = row * m_imageSize.y;

	return glm::ivec4{ srcX, srcY, m_imageSize.x, m_imageSize.y };
}
