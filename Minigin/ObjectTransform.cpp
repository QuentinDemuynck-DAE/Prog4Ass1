#include <iostream>

#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& owner, const glm::vec3& localPosition, const glm::vec3& localRotation, const glm::vec3& localScale)
    : m_localPosition(localPosition)
    , m_localRotation(localRotation)
    , m_localScale(localScale)
    , m_owner(owner)
{
    m_globalPosition = m_localPosition;
    m_globalRotation = m_localRotation;
    m_globalScale = m_localScale;
}

// --- POSITION ---
void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
    SetLocalPosition(glm::vec3{ x,y,z });
}

const glm::vec3& dae::Transform::GetGlobalPosition()
{
    if (m_positionIsDirty)
    {
        UpdateGlobalPosition();
    }
    return m_globalPosition;
}

void dae::Transform::SetGlobalPosition(const float x, const float y, const float z)
{
    SetGlobalPosition(glm::vec3{ x,y,z });
}

void dae::Transform::UpdateGlobalPosition()
{
    if (!m_positionIsDirty) return; // Prevent redundant updates

    if (m_owner.GetParent() == nullptr)
    {
        m_globalPosition = m_localPosition;
    }
    else
    {
        // Ensure parent's position is updated first to prevent infinite recursion
        Transform* parentTransform = m_owner.GetParent()->GetTransform();
        if (parentTransform->m_positionIsDirty)
        {
            parentTransform->UpdateGlobalPosition();
        }

        m_globalPosition = parentTransform->GetGlobalPosition() + m_localPosition;
    }

    m_positionIsDirty = false;
}

// --- ROTATION ---
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
    if (!m_rotationIsDirty) return; // Prevent redundant updates

    if (m_owner.GetParent() == nullptr)
    {
        m_globalRotation = m_localRotation;
    }
    else
    {
        // Ensure parent's rotation is updated first
        Transform* parentTransform = m_owner.GetParent()->GetTransform();
        if (parentTransform->m_rotationIsDirty)
        {
            parentTransform->UpdateGlobalRotation();
        }

        m_globalRotation = parentTransform->GetGlobalRotation() + m_localRotation;
    }

    m_rotationIsDirty = false;
}

// --- SCALE ---
void dae::Transform::SetLocalScale(const float x, const float y, const float z)
{
    SetLocalScale(glm::vec3{ x,y,z });
}

void dae::Transform::SetGlobalScale(const float x, const float y, const float z)
{
    SetGlobalScale(glm::vec3{ x,y,z });
}

const glm::vec3& dae::Transform::GetGlobalScale()
{
    if (m_scaleIsDirty)
    {
        UpdateGlobalScale();
    }
    return m_globalScale;
}

void dae::Transform::UpdateGlobalScale()
{
    if (!m_scaleIsDirty) return; // Prevent redundant updates

    if (m_owner.GetParent() == nullptr)
    {
        m_globalScale = m_localScale;
    }
    else
    {
        // Ensure parent's scale is updated first
        Transform* parentTransform = m_owner.GetParent()->GetTransform();
        if (parentTransform->m_scaleIsDirty)
        {
            parentTransform->UpdateGlobalScale();
        }

        m_globalScale = parentTransform->GetGlobalScale() * m_localScale; // Use multiplication for scale instead of addition
    }

    m_scaleIsDirty = false;
}

// --- DIRTY FLAGS ---
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
