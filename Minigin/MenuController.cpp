#include "MenuController.h"
#include "ButtonComponent.h"
#include "GameObject.h"

namespace dae {

    MenuController::MenuController(GameObject& owner)
        : Component(owner)
        , m_SelectedIndex(0)
    {
    }

    void MenuController::AddButton(ButtonComponent* button)
    {
        m_Buttons.push_back(button);
        if (m_Buttons.size() == 1)
        {
            button->SetSelected(true);
        }
    }

    void MenuController::Up()
    {
        if (m_Buttons.empty()) return;

        m_Buttons[m_SelectedIndex]->SetSelected(false);
        m_SelectedIndex = (m_SelectedIndex + m_Buttons.size() - 1) % m_Buttons.size();
        m_Buttons[m_SelectedIndex]->SetSelected(true);
    }

    void MenuController::Down()
    {
        if (m_Buttons.empty()) return;

        m_Buttons[m_SelectedIndex]->SetSelected(false);
        m_SelectedIndex = (m_SelectedIndex + 1) % m_Buttons.size();
        m_Buttons[m_SelectedIndex]->SetSelected(true);
    }

    void MenuController::ClickCurrent()
    {
        if (m_Buttons.empty()) return;
        m_Buttons[m_SelectedIndex]->Click();
    }

} 