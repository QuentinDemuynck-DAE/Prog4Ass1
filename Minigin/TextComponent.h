#pragma once
#include <string>
#include <memory>
#include <Common/b2Settings.h>

#include "GameObject.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:

		struct Color
		{
			uint8 r;
			uint8 g;
			uint8 b;
		};

		void Update(float deltaTime) override;
		void Render(glm::vec3 position, glm::vec2 scale) override;
		void SetText(const std::string& text);

		void SetColor(const Color& color);

		TextComponent(dae::GameObject& owner, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		Color m_Color {255 ,255,255};
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
