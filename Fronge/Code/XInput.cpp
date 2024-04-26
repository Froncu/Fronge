#include "InputManager.h"

#include "ButtonInput.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xinput.h>

#pragma region Constructors/Destructor
fro::InputManager::InputManager()
	: m_pImplementation{ std::make_unique<InputManager::GameControllerInputImplementation>() }
{
}

fro::InputManager::~InputManager()
{
}
#pragma endregion Constructors/Destructor



#pragma region Implementation
class fro::InputManager::GameControllerInputImplementation final
{
public:
	GameControllerInputImplementation() = default;

	~GameControllerInputImplementation() = default;

	void processGamePadInputContinous(std::map<ButtonInput, std::string> const& mActions,
		std::map<std::string, std::vector<std::unique_ptr<Command>>> const& mCommands)
	{
		updateController();
		executeCommands(mActions, mCommands);
	}

private:
	GameControllerInputImplementation(GameControllerInputImplementation const&) = delete;
	GameControllerInputImplementation(GameControllerInputImplementation&&) noexcept = delete;

	GameControllerInputImplementation& operator=(GameControllerInputImplementation const&) = delete;
	GameControllerInputImplementation& operator=(GameControllerInputImplementation&&) noexcept = delete;

	void updateController()
	{
		XINPUT_STATE currentGamePadState;
		ZeroMemory(&currentGamePadState, sizeof(decltype(currentGamePadState)));
		XInputGetState(0, &currentGamePadState);

		auto const buttonChanges{ currentGamePadState.Gamepad.wButtons ^ m_PreviousGamePadState.Gamepad.wButtons };
		m_PreviousGamePadState = currentGamePadState;

		m_ButtonsPressed = buttonChanges & currentGamePadState.Gamepad.wButtons;
		m_ButtonsReleased = buttonChanges & (~currentGamePadState.Gamepad.wButtons);
		m_ButtonsDown = currentGamePadState.Gamepad.wButtons;
	}

	fro_NODISCARD bool isButtonInState(WORD const button, ButtonInput::State const state) const
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

	fro_NODISCARD WORD convertSDLControllerButtonToXInput(SDL_GameControllerButton const gameControllerButton) const
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

	void executeCommands
	(
		std::map<ButtonInput, std::string> const& mActions,
		std::map<std::string, std::vector<std::unique_ptr<Command>>> const& mCommands
	)
	{
		for (auto const& [buttonInput, actionName] : mActions)
		{
			auto const optionalButton{ buttonInput.getButton<SDL_GameControllerButton>() };
			if (!optionalButton.has_value())
				continue;

			if (!isButtonInState(convertSDLControllerButtonToXInput(*optionalButton), buttonInput.getState()))
				continue;

			auto const actionIterator{ mActions.find(buttonInput) };
			if (actionIterator == mActions.end())
				continue;

			auto const commandIterator{ mCommands.find(actionIterator->second) };
			if (commandIterator == mCommands.end())
				continue;

			for (auto& pCommand : commandIterator->second)
				(*pCommand)();
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