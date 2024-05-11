#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "InputManager.h"
#include "GridMovement.h"
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

#include <sdl_main.h>
#include <sdl_mixer.h>

int main(int, char**)
{
	try
	{
		auto& audioService{ fro::ServiceLocator<fro::AudioService>::getInstance() };

		audioService.setProvider<fro::AudioSDL>();
		audioService.getService().playMusic("BGM #01.mp3", 0.1f);

		fro::RenderContext::getInstance().setScalingMode(fro::RenderContext::ScalingMode::fill);
		fro::RenderContext::getInstance().setWindowResizeable(false);
		fro::RenderContext::getInstance().setResolution(160, 144);
		fro::RenderContext::getInstance().setWindowSize(800, 720);
		fro::RenderContext::getInstance().setWindowTitle("DigDug");

		fro::Scene& scene{ fro::SceneManager::getInstance().addScene() };

		fro::GameObject& player1{ scene.addGameObject() };
		player1.addComponent<fro::Sprite>()->setFileName("DigDug.png");
		player1.getComponent<fro::Transform>()->setLocalTranslation({ 8, 8 });
		player1.addComponent<fro::GridMovement>()->setActionNames("moveRight1", "moveLeft1", "moveUp1", "moveDown1");

		fro::GameObject& player2{ scene.addGameObject() };
		player2.addComponent<fro::Sprite>()->setFileName("Fygar.png");
		player2.getComponent<fro::Transform>()->setLocalTranslation({ 24, 8 });
		player2.addComponent<fro::GridMovement>()->setActionNames("moveRight2", "moveLeft2", "moveUp2", "moveDown2");

		fro::InputManager::getInstance().setActionDeadzone("moveRight1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveLeft1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveUp1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveDown1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveRight2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveLeft2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveUp2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveDown2", 0.25f);

		fro::InputManager::getInstance().bindActionToInput("moveRight1", SDL_SCANCODE_D);
		fro::InputManager::getInstance().bindActionToInput("moveLeft1", SDL_SCANCODE_A);
		fro::InputManager::getInstance().bindActionToInput("moveUp1", SDL_SCANCODE_W);
		fro::InputManager::getInstance().bindActionToInput("moveDown1", SDL_SCANCODE_S);

		fro::InputManager::getInstance().bindActionToInput("moveRight2", fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickRight });
		fro::InputManager::getInstance().bindActionToInput("moveLeft2", fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickLeft });
		fro::InputManager::getInstance().bindActionToInput("moveUp2", fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickUp });
		fro::InputManager::getInstance().bindActionToInput("moveDown2", fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickDown });

		fro::InputManager::getInstance().bindActionToInput("moveRight1", fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickRight });
		fro::InputManager::getInstance().bindActionToInput("moveLeft1", fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickLeft });
		fro::InputManager::getInstance().bindActionToInput("moveUp1", fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickUp });
		fro::InputManager::getInstance().bindActionToInput("moveDown1", fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickDown });

		return fro::Fronge::getInstance().run();
	}
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::darkRed);
		return 1;
	}
}