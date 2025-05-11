#pragma once
#include "Component.h"
#include "Events.h"

namespace dae
{
	class GameObject;

	class State
	{
	public:
		virtual ~State() = default;

		virtual void OnEnter(GameObject&) {}
		virtual void OnExit(GameObject&) {}

		virtual void HandleInput(GameObject& object, const Event& event) = 0;
		virtual void Update(GameObject&, float){};
	};
}
