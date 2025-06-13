#pragma once
#include "Command.h"
#include "Component.h"

namespace dae
{
	class TextComponent;
}

namespace dae
{
	class ButtonComponent :
		public Component
	{
	public:
		ButtonComponent(GameObject& owner, std::unique_ptr<Command> command);
		void Click();
		void SetSelected(bool selected);
	private:
		bool m_Selected;
		std::unique_ptr<Command> m_Command;
		TextComponent* m_TextComponent;
	};

}
