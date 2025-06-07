#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update(float deltaTime);
		void PostUpdate(float deltaTime);
		void FixedUpdate();
		void Render() const;

		const std::string& GetName() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		void RecursiveUpdate(const std::shared_ptr<GameObject>& object, float deltaTime);
		void RecursivePostUpdate(const std::shared_ptr<GameObject>& object, float deltaTime);
		void RecursiveFixedUpdate(const std::shared_ptr<GameObject>& object);
		void RecursiveRender(const std::shared_ptr<GameObject>& object) const;

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
