#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae
{
	class GameObject;
	class Transform final
	{
	public:
		Transform(GameObject& owner, const glm::vec3& localPosition = { 0,0,0 }, const glm::vec3& localRotation = { 0,0,0 }, const glm::vec3& localScale = { 0,0,0 });
		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		void SetLocalPosition(const float x,const float y,const  float z);
		void SetLocalPosition(const glm::vec3& position) { SetPositionDirty(); m_localPosition = position; }

		const glm::vec3& GetGlobalPosition();

		const glm::vec3& GetLocalRotation() const { return m_localRotation; }
		void SetLocalRotation(const float x, const float y, const float z);
		void SetLocalRotation(const glm::vec3& rotation) { SetRotationDirty(); m_localRotation = rotation; }

		const glm::vec3& GetGlobalRotation();

		const glm::vec3& GetLocalScale() const { return m_localScale; }
		void SetLocalScale(const float x, const float y, const float z);
		void SetLocalScale(const glm::vec3& scale) { SetScaleDirty(); m_localScale = scale; }

		const glm::vec3& GetGlobalScale();

		void SetPositionDirty();
		void SetScaleDirty();
		void SetRotationDirty();

	private:
		glm::vec3 m_localPosition;
		glm::vec3 m_globalPosition;
		bool m_positionIsDirty { false };
		void SetGlobalPosition(const float x, const float y, const float z);
		void SetGlobalPosition(const glm::vec3& position) { m_globalPosition = position; }
		void UpdateGlobalPosition();

		glm::vec3 m_localRotation;
		glm::vec3 m_globalRotation;
		bool m_rotationIsDirty{ false };
		void SetGlobalRotation(const float x, const float y, const float z);
		void SetGlobalRotation(const glm::vec3& rotation) { m_globalRotation = rotation; }
		void UpdateGlobalRotation();


		glm::vec3 m_localScale;
		glm::vec3 m_globalScale;
		bool m_scaleIsDirty{ false };
		void SetGlobalScale(const float x, const float y, const float z);
		void SetGlobalScale(const glm::vec3& scale) { m_globalScale = scale; }
		void UpdateGlobalScale();

		GameObject& m_owner;
	};
}
