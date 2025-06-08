#include "Subject.h"

#include "SceneManager.h"

dae::Subject::Subject(GameObject* gameObject)
	:m_GameObject(gameObject)
{

}

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer) {
    if (observer && std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end()) {
        m_Observers.push_back(observer);
    }
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer) {
    auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
    if (it != m_Observers.end()) {
        m_Observers.erase(it);
    }
}

void dae::Subject::Notify(Event event)
{
    if (!SceneManager::GetInstance().IsInActiveScene(m_GameObject))
        return;

    for (auto observer : m_Observers)
    {
        observer.get()->Notify(event, m_GameObject);
    }
}
