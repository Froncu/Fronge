#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "RenderContext.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceLocator.hpp"
#include "Sprite.h"
#include "Steam.h"
#include "SystemEventManager.h"
#include "Text.h"
#include "Timer.h"
#include "Transform.h"

#include <SDL_main.h>
#include <SDL_mixer.h>

int main(int, char**)
{
	try
	{
		auto& audioService{ fro::ServiceLocator<fro::AudioService>::getInstance() };

		audioService.setProvider<fro::AudioSDL>();
		audioService.getService().playMusic("BGM #01.mp3", 0.1f);

		fro::Scene& scene{ fro::SceneManager::getInstance().addScene() };

		fro::GameObject& player1{ scene.addGameObject() };
		player1.addComponent<fro::Sprite>()->setFileName("DigDug.png");
		player1.getComponent<fro::Transform>()->setLocalPosition({ 300, 300 });

		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_D, fro::ButtonInput::State::down }, "moveRight");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_A, fro::ButtonInput::State::down }, "moveLeft");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_W, fro::ButtonInput::State::down }, "moveUp");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_SCANCODE_S, fro::ButtonInput::State::down }, "moveDown");

		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveRight", player1).setMoveDirection({ 1.0f, 0.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveLeft", player1).setMoveDirection({ -1.0f, 0.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveUp", player1).setMoveDirection({ 0.0f, -1.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveDown", player1).setMoveDirection({ 0.0f, 1.0f });

		fro::GameObject& player2{ scene.addGameObject() };
		player2.addComponent<fro::Sprite>()->setFileName("DigDug.png");
		player2.getComponent<fro::Transform>()->setLocalPosition({ 200, 300 });

		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_RIGHT, fro::ButtonInput::State::down }, "moveRightController");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_LEFT, fro::ButtonInput::State::down }, "moveLeftController");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_UP, fro::ButtonInput::State::down }, "moveUpController");
		fro::InputManager::getInstance().bindKeyInputToAction({ SDL_CONTROLLER_BUTTON_DPAD_DOWN, fro::ButtonInput::State::down }, "moveDownController");

		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveRightController", player2).setMoveDirection({ 1.0f, 0.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveLeftController", player2).setMoveDirection({ -1.0f, 0.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveUpController", player2).setMoveDirection({ 0.0f, -1.0f });
		fro::InputManager::getInstance().bindActionToCommand<fro::MoveCommand>("moveDownController", player2).setMoveDirection({ 0.0f, 1.0f });

		return fro::Fronge::getInstance().run();
	}
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::DarkRed);
		return 1;
	}
}