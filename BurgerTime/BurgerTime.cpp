#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "AnimatedSpriteComponent.h"
#include "Collision.h"

#include "CollisionComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "Texture2DComponent.h"
#include "FPSComponent.h"
#include "Subject.h"
#include "ScoreObserver.h"
#include "ScoreComponent.h"
#include "Commands.h"
#include "EnemyComponent.h"
#include "EnemyObserver.h"
#include "PlaySoundCommand.h"
#include "ServiceLocator.h"
#include <glm.hpp> 

#include "AiController.h"
#include "DebugPositionCommand.h"
#include "GamePadController.h"
#include "KeyboardControllerComponent.h"
#include "MapComponent.h"
#include "SVGParser.h"
#include "Prototypes.h"
#include "SceneCreators.h"


void load()
{

	auto noisySoundSystem = std::make_unique<NoisySoundSystem>();
	ServiceLocator::ProvideSoundSystem(std::make_unique<DebugSoundSystem>(std::move(noisySoundSystem)));

	for (int i{} ; i < g_maxControllers; i++)
	{
		dae::InputManager::GetInstance().AddGamePad(std::make_unique<dae::GamePad>(i));
	}

	dae::GameManager::GetInstance().LoadScenes();

	//scene.Add(livesPlayerOne);
	//scene.Add(livesPlayerTwo);
	//
	//scene.Add(scorePlayerOne);
	//scene.Add(scorePlayerTwo);


	//auto livesPlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,80,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//auto scorePlayerOne = std::make_shared<dae::GameObject>(glm::vec3{ 20,95,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//
	//auto livesPlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,110,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//auto scorePlayerTwo = std::make_shared<dae::GameObject>(glm::vec3{ 20,125,0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 2.0f,2.0f,2.0f });
	//
	//livesPlayerOne->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);
	//livesPlayerTwo->AddComponent<dae::TextComponent>("# of lifes: 5", smallerFont);
	//
	//scorePlayerOne->AddComponent<dae::TextComponent>("Score: 0", smallerFont);
	//scorePlayerTwo->AddComponent<dae::TextComponent>("Score: 0", smallerFont);
	//
	//auto healthObserverOne = std::make_shared<HealthObserver>(livesPlayerOne->GetComponent<dae::TextComponent>());
	//auto healthObserverTwo = std::make_shared<HealthObserver>(livesPlayerTwo->GetComponent<dae::TextComponent>());
	//
	//auto scoreObserverOne = std::make_shared<ScoreObserver>(scorePlayerOne->GetComponent<dae::TextComponent>());
	//auto scoreObserverTwo = std::make_shared<ScoreObserver>(scorePlayerTwo->GetComponent<dae::TextComponent>());

	//textureOne.get()->GetSubject()->AddObserver(healthObserverOne);
	//textureTwo.get()->GetSubject()->AddObserver(healthObserverTwo);
	//
	//textureOne.get()->GetSubject()->AddObserver(scoreObserverOne);
	//textureTwo.get()->GetSubject()->AddObserver(scoreObserverTwo);

	//const float speed{ 100.f };
}

int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}