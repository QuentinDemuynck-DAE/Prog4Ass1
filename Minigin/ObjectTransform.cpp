#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& owner, const glm::vec3& localPosition, const glm::vec3& localRotation, const glm::vec3& localScale)
	:m_localPosition(localPosition)
	, m_localRotation(localRotation)
	, m_localScale(localScale)
	, m_owner(owner)
{
	m_globalPosition = m_localPosition;
	m_globalRotation = m_localRotation;
	m_globalScale = m_localScale;
}

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	SetLocalPosition(glm::vec3{x,y,z});
}

const glm::vec3& dae::Transform::GetGlobalPosition()
{
	if (m_positionIsDirty)
	{
		UpdateGlobalPosition();
	}
	return m_globalPosition;
}

void dae::Transform::SetGlobalPosition(const float x,const float y, const float z)
{
	SetGlobalPosition(glm::vec3{ x,y,z });
}

void dae::Transform::UpdateGlobalPosition()
{
	if (m_positionIsDirty)
	{
		if (m_owner.GetParent() == nullptr)
		{
			m_globalPosition = m_localPosition;
		}
		else
		{
			m_globalPosition = m_owner.GetTransform()->GetGlobalPosition() + m_localPosition;
		}
	}
	m_positionIsDirty = false;
}

void dae::Transform::SetLocalRotation(const float x, const float y, const float z)
{
	SetLocalRotation(glm::vec3{ x,y,z });
}

const glm::vec3& dae::Transform::GetGlobalRotation()
{
	if (m_rotationIsDirty)
	{
		UpdateGlobalRotation();
	}
	return m_globalRotation;
}

void dae::Transform::SetGlobalRotation(const float x, const float y, const float z)
{
	SetGlobalRotation(glm::vec3{ x,y,z });
}

void dae::Transform::UpdateGlobalRotation()
{
	if (m_rotationIsDirty)
	{
		if (m_owner.GetParent() == nullptr)
		{
			m_globalRotation = m_localRotation;
		}
		else
		{
			m_globalRotation = m_owner.GetTransform()->GetGlobalRotation() + m_localRotation;
		}
	}
	m_rotationIsDirty = false;
}

void dae::Transform::SetLocalScale(const float x, const float y, const float z)
{
	SetLocalScale(glm::vec3{ x,y,z });
}

void dae::Transform::SetGlobalScale(const float x, const float y, const float z)
{
	SetGlobalScale(glm::vec3{ x,y,z });
}

void dae::Transform::UpdateGlobalScale()
{
	if (m_scaleIsDirty)
	{
		if (m_owner.GetParent() == nullptr)
		{
			m_globalScale = m_localScale;
		}
		else
		{
			m_globalScale = m_owner.GetTransform()->GetGlobalScale() + m_localScale;
		}
	}
	m_scaleIsDirty = false;
}

const glm::vec3& dae::Transform::GetGlobalScale()
{
	if (m_scaleIsDirty)
	{
		UpdateGlobalScale();
	}
	return m_globalScale;
}

void dae::Transform::SetPositionDirty()
{
	m_positionIsDirty = true;
}

void dae::Transform::SetScaleDirty()
{
	m_scaleIsDirty = true;
}

void dae::Transform::SetRotationDirty()
{
	m_rotationIsDirty = true;
}
