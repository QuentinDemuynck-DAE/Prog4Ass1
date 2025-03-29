#pragma once
#include "Component.h"
#include "TextComponent.h"
#include <memory.h>
#include "Font.h"
#include <sstream>
#include <iomanip>
#include "GameObject.h"

class FPSComponent final : public Component
{
public:
	void Update(float deltaTime) override;
	void FixedUpdate() override;
	FPSComponent(dae::GameObject& owner);
	virtual ~FPSComponent() = default;
	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) = delete;

	float GetFPS() const { return m_FPS; }
	void SetShowFPS(bool show) { m_showFPS = show; }
	bool GetShowFPS() const { return m_showFPS; }


private:
	float m_FPS { };
	bool m_showFPS { true };
	int m_counter{ 0 };
	

	dae::TextComponent* m_textComponent = { nullptr };
};

