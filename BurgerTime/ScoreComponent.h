#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;

	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(dae::GameObject& owner, dae::TextComponent* textComponent);
		virtual ~ScoreComponent() = default;

		void FixedUpdate() override;

	private:
		dae::TextComponent* m_TextComponent;
	};

}


