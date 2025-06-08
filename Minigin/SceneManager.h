#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class GameObject;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void PostUpdate(float deltaTime);
		void FixedUpdate();
		void Render();

		void SetActiveScene(const std::string& name);
		void SetActiveScene(std::shared_ptr<Scene> scene);

		bool IsInActiveScene(GameObject* obj) const;

		std::shared_ptr<Scene> GetActiveScene() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_pActiveScene = nullptr;
	};
}
