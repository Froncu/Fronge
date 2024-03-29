#include "InputManager.h"

#include "ButtonInput.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xinput.h>
#include <thread>
#include <iostream>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(InputManager)
	: m_pImplementation{ std::make_unique<InputManager::GameControllerInputImplementation>() }
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(InputManager)
{
}
#pragma endregion Constructors/Destructor



#pragma region Implementation
class fro::InputManager::GameControllerInputImplementation final
{
public:
	GameControllerInputImplementation() = default;
	GameControllerInputImplementation(const GameControllerInputImplementation&) = delete;
	GameControllerInputImplementation(GameControllerInputImplementation&&) noexcept = delete;

	~GameControllerInputImplementation() = default;

	GameControllerInputImplementation& operator=(const GameControllerInputImplementation&) = delete;
	GameControllerInputImplementation& operator=(GameControllerInputImplementation&&) noexcept = delete;

	void processGamePadInputContinous(const std::map<ButtonInput, std::string>& mActions,
		const std::map<std::string, std::vector<std::unique_ptr<Command>>>& mCommands)
	{
		updateController();

		for (auto&& [buttonInput, actionName] : mActions)
		{
			auto optionalButton{ buttonInput.getButton<SDL_GameControllerButton>() };
			if (!optionalButton.has_value())
				continue;

			if (!isButtonInState(convertSDLControllerButtonToXInput(*optionalButton), buttonInput.getState()))
				continue;

			if (const auto actionsIterator{ mActions.find(buttonInput) }; actionsIterator != mActions.end())
				if (const auto commandsIterator{ mCommands.find(actionsIterator->second) }; commandsIterator != mCommands.end())
					for (auto& pCommand : commandsIterator->second)
						(*pCommand)();
		}
	}

private:
	void updateController()
	{
		XINPUT_STATE currentGamePadState;
		ZeroMemory(&currentGamePadState, sizeof(decltype(currentGamePadState)));
		XInputGetState(0, &currentGamePadState);

		const auto buttonChanges{ currentGamePadState.Gamepad.wButtons ^ m_PreviousGamePadState.Gamepad.wButtons };
		m_PreviousGamePadState = currentGamePadState;

		m_ButtonsPressed = buttonChanges & currentGamePadState.Gamepad.wButtons;
		m_ButtonsReleased = buttonChanges & (~currentGamePadState.Gamepad.wButtons);
		m_ButtonsDown = currentGamePadState.Gamepad.wButtons;
	}

	bool isButtonInState(WORD button, ButtonInput::State state) const
	{
		switch (state)
		{
		case ButtonInput::State::pressed:
			return m_ButtonsPressed & button;

		case ButtonInput::State::released:
			return m_ButtonsReleased & button;

		case ButtonInput::State::down:
			return m_ButtonsDown & button;
		}

		return false;
	}

	WORD convertSDLControllerButtonToXInput(SDL_GameControllerButton gameControllerButton)
	{
		switch (gameControllerButton)
		{
		case SDL_CONTROLLER_BUTTON_A:
			return XINPUT_GAMEPAD_A;

		case SDL_CONTROLLER_BUTTON_B:
			return XINPUT_GAMEPAD_B;

		case SDL_CONTROLLER_BUTTON_X:
			return XINPUT_GAMEPAD_X;

		case SDL_CONTROLLER_BUTTON_Y:
			return XINPUT_GAMEPAD_Y;

		case SDL_CONTROLLER_BUTTON_BACK:
			return XINPUT_GAMEPAD_BACK;

		case SDL_CONTROLLER_BUTTON_START:
			return XINPUT_GAMEPAD_START;

		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			return XINPUT_GAMEPAD_LEFT_THUMB;

		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			return XINPUT_GAMEPAD_RIGHT_THUMB;

		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;

		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;

		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			return XINPUT_GAMEPAD_DPAD_UP;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			return XINPUT_GAMEPAD_DPAD_DOWN;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			return XINPUT_GAMEPAD_DPAD_LEFT;

		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			return XINPUT_GAMEPAD_DPAD_RIGHT;

		default:
			return NULL;
		}
	}

	XINPUT_STATE m_PreviousGamePadState{};

	WORD m_ButtonsPressed{};
	WORD m_ButtonsReleased{};
	WORD m_ButtonsDown{};
};
#pragma endregion Implementation



#pragma region PublicMethods
void fro::InputManager::processGamePadInputContinous() const
{
	m_pImplementation->processGamePadInputContinous(m_mActions, m_mCommands);
}
#pragma endregion PublicMethods