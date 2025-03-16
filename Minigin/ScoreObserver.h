#pragma once
#include "Observer.h"
#include "TextComponent.h"

class ScoreObserver : public Observer
{
public:
	ScoreObserver(dae::TextComponent* textComponent);
	virtual void Notify(Event event, dae::GameObject* gameObject) override;
private:
	dae::TextComponent* m_TextComponent;
};

