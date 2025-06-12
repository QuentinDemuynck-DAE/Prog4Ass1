#pragma once
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "Command.h"
#include "Component.h"
#include "Globals.h"

namespace dae
{
    enum class Action
    {
	    Up,
        Left,
        Right,
        Down,
        Shoot,
        Select
    };

    struct Pending {
        enum Type { Bind, Unbind }     type;
        Action         button;
        KeyState         state;        // only used for binds
        std::shared_ptr<Command> cmd;  // only used for binds
    };

    class ControllerComponent : public Component
	{
    public:
        ControllerComponent(GameObject& owner);


        virtual ~ControllerComponent() = default;

        // Im now binding with actions instead of commands so that it doesnt matter what type of controller you have
        void Bind(Action action, std::shared_ptr<Command> cmd, KeyState state = KeyState::Down);
        void Unbind(Action action);
        void FixedUpdate() override;
        void PostUpdate(float deltaTime) override;


    protected:
        // Checks whether the given action is currently active
        virtual bool CheckAction(Action action) = 0;


    private:
        void ProcessPending();

        std::unordered_map<Action, std::pair<KeyState, std::shared_ptr<Command>>> m_Bindings;
        std::unordered_map<Action, bool> m_PreviousState;
        bool m_Notified{ false };
        std::queue<Pending> m_PendingOps;
    };
}

