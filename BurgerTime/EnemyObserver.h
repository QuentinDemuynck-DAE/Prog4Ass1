#pragma once
#include "Observer.h"

class EnemyObserver : public Observer
{
public:
	EnemyObserver();
	virtual void Notify(Event event, dae::GameObject* gameObject) override;
private:
};

