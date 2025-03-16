#pragma once

#include <memory>
#include<vector>
#include "Observer.h"

class GameObject;
namespace dae
{
	class Subject
	{
	public:
		Subject(GameObject* gameObject);

		void AddObserver(std::shared_ptr<Observer> observer);
		void RemoveObserver(std::shared_ptr<Observer> observer);
		void Notify(Event event);

	private:
		GameObject* m_GameObject;

		std::vector<std::shared_ptr<Observer>> m_Observers;
	};
}



