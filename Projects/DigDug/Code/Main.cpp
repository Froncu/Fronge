#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "IdleState.h"
#include "InputManager.h"
#include "MovementRotation.h"
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

#include <sdl_main.h>
#include <sdl_mixer.h>

int main(int, char**)
{
	try
	{
		auto& audioService{ fro::ServiceLocator<fro::AudioService>::getInstance() };
		audioService.setProvider<fro::AudioSDL>();



		auto& renderContext{ fro::RenderContext::getInstance() };

		renderContext.setScalingMode(fro::RenderContext::ScalingMode::fill);
		renderContext.setWindowResizeable(false);
		renderContext.setResolution(160, 144);
		renderContext.setWindowSize(800, 720);
		renderContext.setWindowTitle("DigDug");



		auto const scene{ fro::SceneManager::getInstance().loadScene("Main") };



		auto const pump{ scene.get().addGameObject("Pump") };

		auto spriteAnimator{ pump.get().forceGetComponent<fro::SpriteAnimator>() };

		spriteAnimator.get().addAnimationFrames("shooting", "DigDug/Pump.png", { 48, 16 }, 1, 6);
		spriteAnimator.get().setFramesPerSecond("shooting", 12);
		spriteAnimator.get().setLoop("shooting", false);

		pump.get().setLocalTranslation({ 24, 0 });

		pump.get().setActive(false);



		auto const player1{ scene.get().addGameObject("DigDug") };

		player1.get().setLocalTranslation({ 8, 8 });

		auto const movementRotation{ player1.get().addComponent<fro::MovementRotation>() };

		player1.get().addComponent<fro::GridMovement>().get().
			correctedMoveDirectionChanged.addSubscriber(
				std::bind(
					&fro::MovementRotation::onCorrectedMoveDirectionChanged,
					&movementRotation.get(),
					std::placeholders::_1,
					std::placeholders::_2));

		spriteAnimator = player1.get().forceGetComponent<fro::SpriteAnimator>();

		spriteAnimator.get().addAnimationFrames("walking", "DigDug/Walking.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().addAnimationFrames("attacking", "DigDug/Attacking.png", { 16, 16 }, 1, 1);
		spriteAnimator.get().addAnimationFrames("pumping", "DigDug/Pumping.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().setLoop("pumping", false);
		spriteAnimator.get().setFramesPerSecond("walking", 6);
		spriteAnimator.get().setFramesPerSecond("pumping", 6);

		player1.get().forceGetComponent<fro::StateMachine>().get().setCurrentState(
			player1.get().forceGetComponent<fro::IdleState>());



		pump.get().setParent(player1, false);



		auto const player2{ scene.get().addGameObject("Fygar") };

		player2.get().addComponent<fro::GridMovement>().get();
		player2.get().forceGetComponent<fro::Sprite>().get().setFileName("Fygar.png");
		player2.get().setLocalTranslation({ 40, 8 });


		auto& inputManager{ fro::InputManager::getInstance() };

		inputManager.setActionDeadzone("attack", 0.25f);
		inputManager.setActionDeadzone("moveRight1", 0.25f);
		inputManager.setActionDeadzone("moveLeft1", 0.25f);
		inputManager.setActionDeadzone("moveUp1", 0.25f);
		inputManager.setActionDeadzone("moveDown1", 0.25f);

		inputManager.bindActionToInput("attack", SDL_SCANCODE_SPACE);
		inputManager.bindActionToInput("moveRight1", SDL_SCANCODE_D);
		inputManager.bindActionToInput("moveLeft1", SDL_SCANCODE_A);
		inputManager.bindActionToInput("moveUp1", SDL_SCANCODE_W);
		inputManager.bindActionToInput("moveDown1", SDL_SCANCODE_S);

		inputManager.bindActionToInput("moveRight1",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickRight });
		inputManager.bindActionToInput("moveLeft1",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickLeft });
		inputManager.bindActionToInput("moveUp1",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickUp });
		inputManager.bindActionToInput("moveDown1",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickDown });

		inputManager.setActionDeadzone("moveRight2", 0.25f);
		inputManager.setActionDeadzone("moveLeft2", 0.25f);
		inputManager.setActionDeadzone("moveUp2", 0.25f);
		inputManager.setActionDeadzone("moveDown2", 0.25f);

		inputManager.bindActionToInput("moveRight2", SDL_SCANCODE_D);
		inputManager.bindActionToInput("moveLeft2", SDL_SCANCODE_A);
		inputManager.bindActionToInput("moveUp2", SDL_SCANCODE_W);
		inputManager.bindActionToInput("moveDown2", SDL_SCANCODE_S);

		inputManager.bindActionToInput("moveRight2",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickRight });
		inputManager.bindActionToInput("moveLeft2",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickLeft });
		inputManager.bindActionToInput("moveUp2",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickUp });
		inputManager.bindActionToInput("moveDown2",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickDown });

		return fro::Fronge::getInstance().run();
	}
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::darkRed);
		return 1;
	}
}