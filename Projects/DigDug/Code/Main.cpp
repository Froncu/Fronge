#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "IdleState.h"
#include "InputManager.h"
#include "GridMovement.h"
#include "RenderContext.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceLocator.hpp"
#include "Sprite.h"
#include "SpriteAnimator.h"
#include "StateMachine.h"
#include "Steam.h"
#include "SystemEventManager.h"
#include "Text.h"
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

		fro::Reference<fro::Scene> const scene{ fro::SceneManager::getInstance().addScene() };

		fro::Reference<fro::GameObject> const pump{ scene.get().addGameObject() };
		pump.get().addComponent<fro::Sprite>();
		pump.get().addComponent<fro::SpriteAnimator>().get().addAnimationFrames("shooting", "DigDug/Pump.png", { 48, 16 }, 1, 6);
		pump.get().getComponent<fro::SpriteAnimator>().get().setFramesPerSecond("shooting", 12);
		pump.get().getComponent<fro::SpriteAnimator>().get().setLoop("shooting", false);
		pump.get().getComponent<fro::Transform>().get().setLocalTranslation({ 24, 0 });
		pump.get().setActive(false);

		fro::Reference<fro::GameObject> const player1{ scene.get().addGameObject() };
		player1.get().getComponent<fro::Transform>().get().setLocalTranslation({ 8, 8 });
		player1.get().addComponent<fro::GridMovement>();
		player1.get().addComponent<fro::Sprite>();
		player1.get().addComponent<fro::SpriteAnimator>().get().addAnimationFrames("walking", "DigDug/Walking.png", { 16, 16 }, 2, 1);
		player1.get().getComponent<fro::SpriteAnimator>().get().addAnimationFrames("attacking", "DigDug/Attacking.png", { 16, 16 }, 1, 1);
		player1.get().getComponent<fro::SpriteAnimator>().get().addAnimationFrames("pumping", "DigDug/Pumping.png", { 16, 16 }, 2, 1);
		player1.get().getComponent<fro::SpriteAnimator>().get().setLoop("pumping", false);
		player1.get().getComponent<fro::SpriteAnimator>().get().setFramesPerSecond("walking", 6);
		player1.get().getComponent<fro::SpriteAnimator>().get().setFramesPerSecond("pumping", 6);
		player1.get().addComponent<fro::StateMachine>().get().setCurrentState(std::make_unique<fro::IdleState>(player1));

		pump.get().setParent(player1, false);

		fro::Reference<fro::GameObject> const player2{ scene.get().addGameObject() };
		player2.get().addComponent<fro::Sprite>().get().setFileName("Fygar.png");
		player2.get().getComponent<fro::Transform>().get().setLocalTranslation({ 40, 8 });

		fro::InputManager::getInstance().setActionDeadzone("attack", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveRight1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveLeft1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveUp1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveDown1", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveRight2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveLeft2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveUp2", 0.25f);
		fro::InputManager::getInstance().setActionDeadzone("moveDown2", 0.25f);

		fro::InputManager::getInstance().bindActionToInput("attack", SDL_SCANCODE_SPACE);
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