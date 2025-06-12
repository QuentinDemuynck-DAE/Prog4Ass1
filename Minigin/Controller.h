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
    // I tried to do the same as in unity and other engines where you make a name for a specific action and then bind keys to those actions (Did some research for this but mostly implemented it myself)
    enum class Action
    {
	    Up,
        Left,
        Right,
        Down,
        Shoot,
        Select,
        Debug,
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
        void Update(float deltaTime) override;
        void PostUpdate(float deltaTime) override;

        virtual void PerformAction(Action){}; // Only does somthing on AI controllers but to avoid casting I do it likee this


    protected:
        // Checks whether the given action is currently active
        virtual bool CheckAction(Action action, KeyState keystate) = 0;
        std::unordered_map<Action, std::pair<KeyState, std::shared_ptr<Command>>> m_Bindings;


    private:
        void ProcessPending();

        bool m_Notified{ false };
        std::queue<Pending> m_PendingOps;
    };
}

