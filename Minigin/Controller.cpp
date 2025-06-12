#include "Controller.h"
#include "GameObject.h"
#include "Subject.h"

namespace dae {
	ControllerComponent::ControllerComponent(GameObject& owner)
		:Component(owner)
	{
        
	}

    void ControllerComponent::Bind(Action action, std::shared_ptr<Command> command, KeyState state)
    {
        m_PendingOps.push({ Pending::Bind, action, state, command });
    }

    void ControllerComponent::Unbind(Action action)
    {
        m_PendingOps.push({ Pending::Unbind, action, KeyState::Down, nullptr });
    }

    void ControllerComponent::ProcessPending()
    {
        while (!m_PendingOps.empty())
        {
            const Pending& op = m_PendingOps.front();

            if (op.type == Pending::Bind)
            {
                m_Bindings[op.button] = { op.state, op.cmd };
            }
            else // Pending::Unbind
            {
                m_Bindings.erase(op.button);
            }

            m_PendingOps.pop();
        }
    }

    void ControllerComponent::Update(float)
    {
        for (auto& [action, binding] : m_Bindings)
        {
            const auto& [state, cmd] = binding;
            if (CheckAction(action, state) && cmd)
                cmd->Execute();
        }
        ProcessPending();
    }

    void ControllerComponent::PostUpdate(float)
    {
        if (!m_Notified)
        {
            Event event = Event(make_sdbm_hash("controller_added"));
            GetOwner().GetSubject()->Notify(event);
            m_Notified = false;
        }

  
    }
} // namespace dae