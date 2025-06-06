#include "SceneManager.h"

#include <iostream>

#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_pActiveScene->Update(deltaTime);
}

void dae::SceneManager::PostUpdate(float deltaTime)
{
	m_pActiveScene->PostUpdate(deltaTime);
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
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const std::shared_ptr<Scene>& scene) {return (scene.get()->GetName() == name); });

	if (it != m_scenes.end())
	{
		m_pActiveScene = *it;
		std::cout << "Now active scene: " << name << std::endl;

	}
	else
	{
		std::cout << "Scene not found: " << name << std::endl;
	}
}

void dae::SceneManager::SetActiveScene(std::shared_ptr<Scene> scene)
{
	m_pActiveScene = scene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
