#include "PlayerObserver.h"

#include "PlayerComponent.h"
#include "GameObject.h"

void dae::PlayerObserver::Notify(Event event, dae::GameObject* gameObject)
{
	gameObject->GetComponent<PlayerComponent>()->HandleInput(event);
}
