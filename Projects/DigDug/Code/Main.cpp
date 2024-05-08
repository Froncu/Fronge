#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "InputManager.h"
#include "Movement.h"
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
		fro::RenderContext::getInstance().setWindowSize(1000, 900);

		fro::Scene& scene{ fro::SceneManager::getInstance().addScene() };

		fro::GameObject& player1{ scene.addGameObject() };
		player1.addComponent<fro::Sprite>()->setFileName("DigDug.png");
		player1.getComponent<fro::Transform>()->setLocalTranslation({ 8, 8 });
		player1.addComponent<fro::Movement>();

		fro::GameObject& player2{ scene.addGameObject() };
		player2.addComponent<fro::Sprite>()->setFileName("DigDug.png");
		player2.getComponent<fro::Transform>()->setLocalTranslation({ 24, 8 });

		fro::InputManager::getInstance().bindActionToInput("moveRightDigDug", SDL_SCANCODE_D);
		fro::InputManager::getInstance().bindActionToInput("moveLeftDigDug", SDL_SCANCODE_A);
		fro::InputManager::getInstance().bindActionToInput("moveUpDigDug", SDL_SCANCODE_W);
		fro::InputManager::getInstance().bindActionToInput("moveDownDigDug", SDL_SCANCODE_S);

		fro::InputManager::getInstance().bindActionToInput("moveRightDigDug", fro::InputManager::JoypadInput{ 0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT });
		fro::InputManager::getInstance().bindActionToInput("moveLeftDigDug", fro::InputManager::JoypadInput{ 0, SDL_CONTROLLER_BUTTON_DPAD_LEFT });
		fro::InputManager::getInstance().bindActionToInput("moveUpDigDug", fro::InputManager::JoypadInput{ 0, SDL_CONTROLLER_BUTTON_DPAD_UP });
		fro::InputManager::getInstance().bindActionToInput("moveDownDigDug", fro::InputManager::JoypadInput{ 0, SDL_CONTROLLER_BUTTON_DPAD_DOWN });

		return fro::Fronge::getInstance().run();
	}
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::darkRed);
		return 1;
	}
}