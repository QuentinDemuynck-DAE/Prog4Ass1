#pragma once
#include "Component.h"
#include "TextComponent.h"

class LivesComponent : public Component
{
public:
	void Update(float deltaTime) override;


	LivesComponent(dae::GameObject& owner, int maxLives);
	virtual ~LivesComponent() = default;
	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) = delete;

	void LoseLive(int amount = 1);

private:

	int m_Lives;
	int m_MaxLives;

	dae::TextComponent* m_TextComponent = { nullptr };
};

