#pragma once
#include "Observer.h"

namespace dae
{
	class MapWalkerComponent;

	class MapTileWalkerObserver : public Observer
	{
	public:
		MapTileWalkerObserver(MapWalkerComponent* mapWalker);
		void Notify(Event event, dae::GameObject* gameObject) override;

		dae::MapWalkerComponent* m_MapWalkerComponent;
	};
}



