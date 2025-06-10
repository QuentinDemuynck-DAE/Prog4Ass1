#pragma once
#include "Observer.h"

namespace dae
{
	class PlayerObserver : public Observer
	{
	public:
		PlayerObserver() = default;
		void Notify(Event event, dae::GameObject* gameObject) override;
	};
}


