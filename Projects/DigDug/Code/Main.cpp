#include "AudioSDL.h"
#include "AudioService.h"
#include "Console.hpp"
#include "FPSCounter.h"
#include "Fronge.h"
#include "GameObject.h"
#include "GUIContext.h"
#include "DigDugIdleState.h"
#include "InputManager.h"
#include "MovementRotation.h"
#include "GridMovement.h"
#include "RenderContext.h"
#include "RigidBody.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceLocator.hpp"
#include "Sprite.h"
#include "SpriteAnimator.h"
#include "StateMachine.h"
#include "SystemEventManager.h"
#include "Text.h"
#include "PhysicsManager.h"

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



		auto const scene{ fro::SceneManager::getInstance().addScene("level1") };
		fro::SceneManager::getInstance().setActiveScene(scene);



		auto const digDug{ scene.get().forceGetGameObject("digDug") };

		digDug.get().setLocalTranslation({ 8, 8 });

		auto const movementRotation{ digDug.get().forceGetComponent<fro::MovementRotation>() };

		digDug.get().forceGetComponent<fro::GridMovement>().get().
			correctedMoveDirectionChanged.addSubscriber(
				std::bind(
					&fro::MovementRotation::onCorrectedMoveDirectionChanged,
					&movementRotation.get(),
					std::placeholders::_1,
					std::placeholders::_2));

		auto spriteAnimator{ digDug.get().forceGetComponent<fro::SpriteAnimator>() };

		spriteAnimator.get().addAnimationFrames("walking", "DigDug/Walking.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().addAnimationFrames("attacking", "DigDug/Attacking.png", { 16, 16 }, 1, 1);
		spriteAnimator.get().addAnimationFrames("pumping", "DigDug/Pumping.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().addAnimationFrames("dead", "DigDug/Dead.png", { 16, 16 }, 5, 1);
		spriteAnimator.get().setLoop("pumping", false);
		spriteAnimator.get().setFramesPerSecond("walking", 6);
		spriteAnimator.get().setFramesPerSecond("pumping", 6);
		spriteAnimator.get().setFramesPerSecond("dead", 3);
		spriteAnimator.get().setLoop("dead", false);
		
		auto const idleState{ digDug.get().forceGetComponent<fro::DigDugIdleState>() };
		digDug.get().forceGetComponent<fro::StateMachine>().get().setCurrentState(idleState);

		digDug.get().forceGetComponent<fro::RigidBody>().get().setColliderSize({ 8, 8 });



		auto const pump{ scene.get().forceGetGameObject("pump") };

		spriteAnimator = pump.get().forceGetComponent<fro::SpriteAnimator>();

		spriteAnimator.get().addAnimationFrames("shooting", "DigDug/Pump.png", { 48, 16 }, 1, 6);
		spriteAnimator.get().setFramesPerSecond("shooting", 12);
		spriteAnimator.get().setLoop("shooting", false);

		pump.get().setLocalTranslation({ 24, 0 });

		pump.get().setActive(false);

		pump.get().setParent(digDug, false);



		auto const pumpHitBox{ scene.get().forceGetGameObject("pumpHitBox") };

		auto rigidBody{ pumpHitBox.get().forceGetComponent<fro::RigidBody>() };

		rigidBody.get().setColliderSize({ 2, 2 });

		pumpHitBox.get().setParent(pump, false);



		auto fygar{ scene.get().forceGetGameObject("fygar") };

		fygar.get().setTag("enemy");

		spriteAnimator = fygar.get().forceGetComponent<fro::SpriteAnimator>().get();

		spriteAnimator.get().addAnimationFrames("walking", "Fygar/Walking.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().addAnimationFrames("charging", "Fygar/Charging.png", { 16, 16 }, 2, 1);
		spriteAnimator.get().addAnimationFrames("dying", "Fygar/Dying.png", { 24, 24 }, 3, 1);
		spriteAnimator.get().setFramesPerSecond("walking", 3);
		spriteAnimator.get().setFramesPerSecond("charging", 3);
		spriteAnimator.get().setFramesPerSecond("dying", 2);

		fygar.get().setLocalTranslation({ 40, 8 });
		fygar.get().forceGetComponent<fro::RigidBody>().get().setColliderSize({ 8, 8 });

		fygar.get().addComponent<fro::GridMovement>().get();



		auto fygarFire{ scene.get().addGameObject("fygarFire") };

		spriteAnimator = fygarFire.get().forceGetComponent<fro::SpriteAnimator>();

		spriteAnimator.get().addAnimationFrames("fireSpit", "Fygar/Fire.png", { 40, 16 }, 1, 3);
		spriteAnimator.get().setFramesPerSecond("fireSpit", 2);
		spriteAnimator.get().setLoop("fireSpit", false);

		fygarFire.get().setLocalTranslation({ 24, 0 });

		fygarFire.get().setActive(true);

		fygarFire.get().setParent(fygar, false);




		auto fygarFireHitBox{ scene.get().addGameObject("fygarFireHitBox") };

		rigidBody = fygarFireHitBox.get().forceGetComponent<fro::RigidBody>();

		rigidBody.get().setColliderSize({ 0, 16 });

		fygarFireHitBox.get().setParent(fygarFire, false);



		auto& inputManager{ fro::InputManager::getInstance() };

		inputManager.bindActionToInput("attackDigDug", SDL_SCANCODE_SPACE);
		inputManager.bindActionToInput("moveRightDigDug", SDL_SCANCODE_D);
		inputManager.bindActionToInput("moveLeftDigDug", SDL_SCANCODE_A);
		inputManager.bindActionToInput("moveUpDigDug", SDL_SCANCODE_W);
		inputManager.bindActionToInput("moveDownDigDug", SDL_SCANCODE_S);

		inputManager.bindActionToInput("attackDigDug",
			fro::InputManager::JoypadInput{ 1, SDL_CONTROLLER_BUTTON_A });
		inputManager.bindActionToInput("moveRightDigDug",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickRight });
		inputManager.bindActionToInput("moveLeftDigDug",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickLeft });
		inputManager.bindActionToInput("moveUpDigDug",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickUp });
		inputManager.bindActionToInput("moveDownDigDug",
			fro::InputManager::JoypadInput{ 1, fro::InputManager::JoypadAxis::leftStickDown });

		inputManager.bindActionToInput("attackFygar",
			fro::InputManager::JoypadInput{ 0, SDL_CONTROLLER_BUTTON_A });
		inputManager.bindActionToInput("moveRightFygar",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickRight });
		inputManager.bindActionToInput("moveLeftFygar",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickLeft });
		inputManager.bindActionToInput("moveUpFygar",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickUp });
		inputManager.bindActionToInput("moveDownFygar",
			fro::InputManager::JoypadInput{ 0, fro::InputManager::JoypadAxis::leftStickDown });

		return fro::Fronge::getInstance().run();
	}
	catch (std::runtime_error const& exception)
	{
		fro::Console::getInstance().log(exception.what(), fro::Console::BackgroundColor::darkRed);
		return 1;
	}
}