#include "SceneManager.h"

#include <iostream>

#include "GameObject.h"
#include "Globals.h"
#include "Scene.h"
#include "Subject.h"
#include "../BurgerTime/GameManager.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_pActiveScene->Update(deltaTime);
}

void dae::SceneManager::PostUpdate(float deltaTime)
{
	m_pActiveScene->PostUpdate(deltaTime);
    if (GameManager::GetInstance().ShouldReset())
    {
        GameManager::GetInstance().Reset();
    }
}

void dae::SceneManager::FixedUpdate()
{
	m_pActiveScene->FixedUpdate();
}

void dae::SceneManager::Render()
{
	m_pActiveScene->Render();
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
    for (size_t i = 0; i < m_scenes.size(); ++i)
    {
        if (m_scenes[i]->GetName() == name)
        {
            SetActiveSceneByIndex(i);
            return;
        }
    }
    throw std::runtime_error("Scene not found: " + name);
}

void dae::SceneManager::SetActiveScene(std::shared_ptr<Scene> scene)
{
    for (size_t i = 0; i < m_scenes.size(); ++i)
    {
        if (m_scenes[i] == scene)
        {
            SetActiveSceneByIndex(i);
            return;
        }
    }
    throw std::runtime_error("Scene pointer not managed by SceneManager");
}

void dae::SceneManager::SetActiveSceneByIndex(size_t index)
{
    if (index >= m_scenes.size())
    {
        throw std::out_of_range("Scene index out of range");
    }
    m_activeSceneIndex = index;
    m_pActiveScene = m_scenes[index];

    for (auto& gameObject : m_pActiveScene.get()->GetObjects())
    {
        Event event = Event(make_sdbm_hash("scene_loaded"));
        gameObject.get()->GetSubject()->Notify(event);
    }
}

void dae::SceneManager::LoadNextScene()
{
    if (m_scenes.empty())
        return;

    size_t nextIndex = (m_activeSceneIndex + 1) % m_scenes.size();
    SetActiveSceneByIndex(nextIndex);
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}

//Helper to know wheter an observer is in the current Scene
bool dae::SceneManager::IsInActiveScene(GameObject* obj) const
{
	if (!m_pActiveScene) return false;
	return m_pActiveScene->Contains(obj);
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetSceneAtIndex(size_t index) const
{
    if (index >= m_scenes.size())
    {
        throw std::out_of_range{ "SceneManager::GetSceneAtIndex: index out of range" };
    }
    return m_scenes[index];
}
dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
