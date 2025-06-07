#pragma once
#include <memory>

#include "Component.h"
#include "Texture2D.h"

namespace dae
{
	class AnimatedSpriteComponent : public Component
	{
	public:
		AnimatedSpriteComponent(dae::GameObject& owner, const std::string& filename, const int& startFrame, const int& endFrame,const glm::ivec2& imageSize, const glm::ivec2 gridSize, const float& frameDelay);

		void SetTexture(const std::string& filename);
		void SetStartFrame(const int& startFrame);
		void SetEndFrame(const int& endFrame);
		void SetColumnOrderedAnimation(const bool& colorOrderedAnimation);
		void Loop(const bool& loop);
		void Pause();
		void Resume();

		virtual ~AnimatedSpriteComponent() = default;
		AnimatedSpriteComponent(const Component& other) = delete;
		AnimatedSpriteComponent(AnimatedSpriteComponent&& other) = delete;
		AnimatedSpriteComponent& operator=(const AnimatedSpriteComponent& other) = delete;
		AnimatedSpriteComponent& operator=(AnimatedSpriteComponent&& other) = delete;

		virtual void Update(float deltaTime);
		virtual void Render(glm::vec3 position, glm::vec2 scale);

	private:

		glm::ivec4 calculateSourceRect();

		glm::ivec2 m_imageSize{ 16,16 };
		glm::ivec2 m_gridSize{ 10,15 };
		int m_startFrame{ 0 };
		int m_endFrame{ 1 };
		const float m_frameDelay{ 0.5f };
		bool m_columnOrderedAnimation{ false }; // for when the frames should go down via column instead of via row (there is one like this in the spritesheet)
		bool m_loop{ true };
		bool m_isPlaying{ true };
		std::shared_ptr<dae::Texture2D> m_texture;

		int m_CurrentFrame{ -1 };
		float m_Timer;
	};
}


