#pragma once
#include "Events.h"
namespace dae
{
	class GameObject;
}

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify(Event event, dae::GameObject* gameObject) = 0;
};
