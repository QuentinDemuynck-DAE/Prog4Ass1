#pragma once
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <iostream>
#include "Globals.h"
#include "Collision.h"

class CollisionListener;

namespace dae
{
	class Minigin
	{
	public:
		static std::unique_ptr<b2World> physicsWorld;
		static bool m_PhysicsPaused;

		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;


	private:
		std::unique_ptr<CollisionListener> m_CollisionListenerPtr;
	};
}
