#pragma once
#include "Controller.h"

namespace dae
{
	class AiController : public ControllerComponent
	{
	public:
		AiController(GameObject& owner) : ControllerComponent(owner)
		{
			
		}
		void PerformAction(Action action) override
		{
			auto it = m_Bindings.find(action);
			if (it == m_Bindings.end()) return;

			it->second.second->Execute();
		}

	protected:
		bool CheckAction(Action, KeyState) override
		{
			return false; // Always false you have to manually call perform
		}
	};
}

