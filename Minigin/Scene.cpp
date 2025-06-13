#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[&](const std::shared_ptr<GameObject>& o)
			{
				return o == object;
			}),
		m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_objects)
	{
		RecursiveUpdate(object, deltaTime);
	}
}

void Scene::RecursiveUpdate(const std::shared_ptr<GameObject>& object, float deltaTime)
{
	object->Update(deltaTime);

	int childCount = object->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		RecursiveUpdate(object->GetChildAt(i), deltaTime);
	}
}

void Scene::PostUpdate(float deltaTime)
{
	auto it = m_objects.begin();
	while (it != m_objects.end())
	{
		if ((*it)->Destroyed())
		{
			(*it)->Destroy();
			it = m_objects.erase(it);
		}
		else
		{
			RecursivePostUpdate(*it, deltaTime);
			++it;
		}
	}

}

void Scene::RecursivePostUpdate(const std::shared_ptr<GameObject>& object, float deltaTime)
{
	object->PostUpdate(deltaTime);

	int childCount = object->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		auto child = object->GetChildAt(i);
		if (child->Destroyed())
		{
			child->Destroy();
		}
		else
		{
			RecursivePostUpdate(child, deltaTime);
		}
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		RecursiveFixedUpdate(object);
	}
}

void Scene::RecursiveFixedUpdate(const std::shared_ptr<GameObject>& object)
{
	object->FixedUpdate();

	int childCount = object->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		RecursiveFixedUpdate(object->GetChildAt(i));
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		RecursiveRender(object);
	}
}

bool Scene::Contains(const GameObject* object) const //go up the root
{
	for (const auto* current = object; current != nullptr; current = current->GetParent())
	{
		for (const auto& rootPtr : m_objects)
		{
			if (rootPtr.get() == current)
				return true;
		}
	}
	return false;
}


void Scene::RecursiveRender(const std::shared_ptr<GameObject>& object) const
{
	object->Render();

	int childCount = object->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		RecursiveRender(object->GetChildAt(i));
	}
}


const std::string& Scene::GetName() const
{
	return m_name;
}

