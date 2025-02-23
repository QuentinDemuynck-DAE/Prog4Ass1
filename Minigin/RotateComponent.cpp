#include "RotateComponent.h"
#include "GameObject.h"
#include <glm.hpp>


RotateComponent::RotateComponent(dae::GameObject& owner, const float speed, bool clockwise)
	:Component(owner), m_speed(speed), m_clockwise(clockwise)
{

}

void RotateComponent::Update(float deltaTime)
{
    // Get the owner's transform
    auto* ownerTransform = GetOwner().GetTransform();
    if (!ownerTransform) return;

    // If there is no parent, no rotation is needed
    auto* parent = GetOwner().GetParent();
    if (!parent) return;

    auto* parentTransform = parent->GetTransform();
    if (!parentTransform) return;

    // Rotation speed (degrees per second)
    float angle = glm::radians(m_speed * deltaTime); 
    if (m_clockwise)
    {
        angle = -angle;
    }

    float cosA = glm::cos(angle);
    float sinA = glm::sin(angle);

    glm::mat3 rotationMatrix = glm::mat3(
        cosA, -sinA, 0.0f,
        sinA, cosA, 0.0f,
        0.0f, 0.0f, 1.0f
    );

    glm::vec3 localPosition = ownerTransform->GetLocalPosition();

    glm::vec3 rotatedPosition = rotationMatrix * localPosition;

    ownerTransform->SetLocalPosition(rotatedPosition);
}


