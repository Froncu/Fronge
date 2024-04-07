#include "Fronge.h"

#include "EventManager.h"
#include "SceneManager.h"
#include "GUIContext.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rotator.h"
#include "Text.h"
#include "FPSCounter.h"
#include "Plot.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "Console.h"
#include "RenderContext.h"
#include "Steam.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <VLD/vld.h>
#include <cassert>

#pragma region EntryFunction
int main(int, char**)
{
	return fro::Fronge::getInstance().run();
}
#pragma endregion EntryFunction



#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(Fronge)
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(Fronge)
{
	SDL_Quit();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
int fro::Fronge::run()
{
	constexpr bool loadSpiral{ true };
	constexpr bool loadFPSCounter{ true };

	Scene& scene{ SceneManager::getInstance().addScene("Test") };

	[[maybe_unused]] GameObject* pGameObject;

	if constexpr (loadSpiral)
	{
		pGameObject = &scene.addGameObject();
		pGameObject->addComponent<Sprite>()->setFileName("logo.tga");
		pGameObject->getComponent<Transform>()->setLocalPosition({ 320, 240 });

		for (size_t index{}; index < 500; ++index)
		{
			constexpr float basePeriod{ 2.0f };
			constexpr float childPeriodDelayMultiplier{ 0.002f };

			GameObject& gameObject{ scene.addGameObject() };
			gameObject.addComponent<Sprite>()->setFileName("logo.tga");
			Rotator* const pRotator{ gameObject.addComponent<Rotator>() };
			pRotator->setPeriod(basePeriod + index * childPeriodDelayMultiplier);
			pRotator->setRadius(2.0f);

			gameObject.setParent(pGameObject, false);
			pGameObject = &gameObject;
		}
	}

	if constexpr (loadFPSCounter)
	{
		pGameObject = &scene.addGameObject();
		Text& text{ *pGameObject->addComponent<Text>() };
		text.setFont("Lingua.otf", 64);
		pGameObject->addComponent<FPSCounter>();
		pGameObject->getComponent<Transform>()->setLocalPosition({ 320, 30 });
	}

	GameObject& player1{ scene.addGameObject() };
	player1.addComponent<Sprite>()->setFileName("DigDug.png");
	player1.getComponent<Transform>()->setLocalPosition({ 300, 300 });

	InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_D, ButtonInput::State::down }, "moveRight");
	InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_A, ButtonInput::State::down }, "moveLeft");
	InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_W, ButtonInput::State::down }, "moveUp");
	InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_S, ButtonInput::State::down }, "moveDown");

	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveRight", player1).setMoveDirection({ 1.0f, 0.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveLeft", player1).setMoveDirection({ -1.0f, 0.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveUp", player1).setMoveDirection({ 0.0f, -1.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveDown", player1).setMoveDirection({ 0.0f, 1.0f });

	GameObject& player2{ scene.addGameObject() };
	player2.addComponent<Sprite>()->setFileName("DigDug.png");
	player2.getComponent<Transform>()->setLocalPosition({ 200, 300 });

	InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ButtonInput::State::down }, "moveRightController");
	InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_LEFT, ButtonInput::State::down }, "moveLeftController");
	InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_UP, ButtonInput::State::down }, "moveUpController");
	InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_DOWN, ButtonInput::State::down }, "moveDownController");

	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveRightController", player2).setMoveDirection({ 1.0f, 0.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveLeftController", player2).setMoveDirection({ -1.0f, 0.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveUpController", player2).setMoveDirection({ 0.0f, -1.0f });
	InputManager::getInstance().bindActionToCommand<MoveCommand>("moveDownController", player2).setMoveDirection({ 0.0f, 1.0f });

	while (true)
	{
		Steam::getInstance().update();

		Timer::getInstance().update();

		if (!EventManager::getInstance().processEvents())
			break;

		SceneManager::getInstance().update();

		RenderContext::getInstance().clear();
		SceneManager::getInstance().render();

		GUIContext::getInstance().startFrame();
		SceneManager::getInstance().display();

		GUIContext::getInstance().endFrame();
		RenderContext::getInstance().present();
	}

	return 0;
}
#pragma endregion PublicMethods