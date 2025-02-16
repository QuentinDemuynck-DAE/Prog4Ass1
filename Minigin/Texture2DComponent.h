#pragma once
#include "Component.h"
#include <string>
#include "Texture2D.h"
#include <memory>
#include <glm.hpp>
#include "Renderer.h"
#include "ResourceManager.h"

class Texture2DComponent final : public Component
{
public:
	void Render(glm::vec3 position) const override;
	void SetTexture(const std::string& filename);

	Texture2DComponent(const std::string& filename);
	virtual ~Texture2DComponent() = default;
	Texture2DComponent(const Texture2DComponent& other) = delete;
	Texture2DComponent(Texture2DComponent&& other) = delete;
	Texture2DComponent& operator=(const Texture2DComponent& other) = delete;
	Texture2DComponent& operator=(Texture2DComponent&& other) = delete;
private:

	std::shared_ptr<dae::Texture2D> m_texture;
};

