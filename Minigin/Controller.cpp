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
                m_PreviousState.erase(op.button);
            }

            m_PendingOps.pop();
        }
    }

    void ControllerComponent::FixedUpdate()
    {
        for (auto& [action, pair] : m_Bindings)
        {
            auto& [state, cmd] = pair;
            bool isPressed = CheckAction(action);
            bool wasPressed = m_PreviousState[action];

            bool shouldExecute = false;

            switch (state)
            {
            case KeyState::Pressed: shouldExecute = isPressed; break;
            case KeyState::Down:    shouldExecute = isPressed && !wasPressed; break;
            case KeyState::Up:      shouldExecute = wasPressed && !isPressed; break;
            }

            if (shouldExecute && cmd)
                cmd->Execute();

            m_PreviousState[action] = isPressed;
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