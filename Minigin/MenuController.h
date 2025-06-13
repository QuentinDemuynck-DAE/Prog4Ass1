// MenuController.h
#pragma once

#include "Component.h"
#include <vector>

namespace dae {

    class ButtonComponent;

    class MenuController : public Component
    {
    public:
        explicit MenuController(GameObject& owner);
        ~MenuController() = default;

        void AddButton(ButtonComponent* button);
        void Up();
        void Down();
        void ClickCurrent();

    private:
        std::vector<ButtonComponent*> m_Buttons;
        size_t m_SelectedIndex{ 0 };
    };
}