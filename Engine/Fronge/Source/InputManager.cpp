#include "InputManager.h"

#include <sdl.h>
#include <sdl_events.h>

#include <stdexcept>

#pragma region Constructors/Destructor
fro::InputManager::InputManager()
{
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
}

fro::InputManager::~InputManager()
{
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::InputManager::processInputContinous()
{
	for (auto& [input, info] : m_mInputs)
		info.state = InputInfo::State::none;
}

void fro::InputManager::processInputEvent(SDL_Event const& event)
{
	auto const& eventType{ event.type };

	switch (eventType)
	{
	case SDL_KEYDOWN:
		if (event.key.repeat)
			return;

		[[fallthrough]];

	case SDL_KEYUP:
		setInputState(eventType == SDL_KEYDOWN,
			event.key.keysym.scancode);
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		setInputState(eventType == SDL_MOUSEBUTTONDOWN,
			static_cast<MouseButton>(event.button.button));
		break;

	case SDL_MOUSEWHEEL:
		// TODO: add support for handling mouse wheel input

		// END TODO
		break;

	case SDL_CONTROLLERDEVICEADDED:
		m_mpJoypadInstanceDeviceIDs.emplace(SDL_JoystickGetDeviceInstanceID(event.cdevice.which), event.cdevice.which);
		m_mpJoypads.emplace(SDL_JoystickGetDeviceInstanceID(event.cdevice.which),
			SDLUniquePointer<SDL_GameController>(SDL_GameControllerOpen(event.cdevice.which), SDL_GameControllerClose));
		break;

	case SDL_CONTROLLERDEVICEREMOVED:
		m_mpJoypadInstanceDeviceIDs.erase(event.cdevice.which);
		m_mpJoypads.erase(event.cdevice.which);
		break;

	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
		setInputState(eventType == SDL_CONTROLLERBUTTONDOWN,
			JoypadInput(m_mpJoypadInstanceDeviceIDs.at(event.cbutton.which),
				static_cast<SDL_GameControllerButton>(event.cbutton.button)));
		break;

	case SDL_CONTROLLERAXISMOTION:
	{
		auto constexpr highestAxisValue{ std::numeric_limits<decltype(event.caxis.value)>::max() };
		auto constexpr lowestAxisValue{ std::numeric_limits<decltype(event.caxis.value)>::lowest() };

		float const inputStrength
		{
			static_cast<float>(event.caxis.value) / (event.caxis.value > 0 ? highestAxisValue : lowestAxisValue)
		};

		auto const joypadID{ m_mpJoypadInstanceDeviceIDs.at(event.cbutton.which) };

		switch (event.caxis.axis)
		{
		case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX:
		case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY:
		case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY:
		case SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX:
			if (event.caxis.value > 0)
			{
				setInputState(inputStrength,
					JoypadInput(joypadID,
						SDLToJoypadStick(event.caxis.value, event.caxis.axis)));

				setInputState(0.0f,
					JoypadInput(joypadID,
						SDLToJoypadStick(lowestAxisValue, event.caxis.axis)));
			}
			else if (event.caxis.value < 0)
			{
				setInputState(inputStrength,
					JoypadInput(joypadID,
						SDLToJoypadStick(event.caxis.value, event.caxis.axis)));

				setInputState(0.0f,
					JoypadInput(joypadID,
						SDLToJoypadStick(highestAxisValue, event.caxis.axis)));
			}
			else
			{
				setInputState(0.0f,
					JoypadInput(joypadID,
						SDLToJoypadStick(highestAxisValue, event.caxis.axis)));

				setInputState(0.0f,
					JoypadInput(joypadID,
						SDLToJoypadStick(lowestAxisValue, event.caxis.axis)));
			}
			break;

		default:
			setInputState(inputStrength,
				JoypadInput(joypadID,
					SDLToJoypadTrigger(event.caxis.axis)));
		}
	}
	}
}

void fro::InputManager::bindActionToInput(std::string const& actionName, Input const input)
{
	m_mActions[actionName].insert(&m_mInputs[input]);
}

float fro::InputManager::getInputStrength(Input const input)
{
	return m_mInputs[input].strength;
}

float fro::InputManager::getActionStrength(std::string const& actionName)
{
	auto const& spBoundInputInfos{ m_mActions[actionName] };

	float largestStrength{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
	{
		if (pBoundInputInfo->strength > largestStrength)
		{
			largestStrength = pBoundInputInfo->strength;
			if (largestStrength == 1.0f)
				break;
		};
	}

	return largestStrength;
}

float fro::InputManager::getActionStrengthAxis1D(std::string const& positiveActionName, std::string const& negativeActionName)
{
	return getActionStrength(positiveActionName) - getActionStrength(negativeActionName);
}

glm::vec2 fro::InputManager::getActionStrengthAxis2D(std::string const& positiveActionNameX,
	std::string const& negativeActionNameX,
	std::string const& positiveActionNameY,
	std::string const& negativeActionNameY)
{
	glm::vec2 const actionStrength
	{
		getActionStrengthAxis1D(positiveActionNameX, negativeActionNameX),
		getActionStrengthAxis1D(positiveActionNameY, negativeActionNameY)
	};

	if (float const actionStrengthMagnitude{ glm::length(actionStrength) };
		actionStrengthMagnitude > 1.0f)
		return actionStrength / actionStrengthMagnitude;

	return actionStrength;
}

bool fro::InputManager::isInputJustPressed(Input const input)
{
	auto const& [strength, state] { m_mInputs[input] };
	return state == InputInfo::State::justPressed;
}

bool fro::InputManager::isActionJustPressed(std::string const& actionName)
{
	auto const& spBoundInputInfos{ m_mActions[actionName] };

	bool justPressedInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
		if (pBoundInputInfo->strength > 0.0f)
			if (pBoundInputInfo->state == InputInfo::State::justPressed)
				justPressedInputPresent = true;
			else
				return false;

	return justPressedInputPresent;
}

bool fro::InputManager::isInputJustReleased(Input const input)
{
	auto const& [strength, state] { m_mInputs[input] };
	return state == InputInfo::State::justReleased;
}

bool fro::InputManager::isActionJustReleased(std::string const& actionName)
{
	auto const& spBoundInputInfos{ m_mActions[actionName] };

	bool justReleasedInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
	{
		if (pBoundInputInfo->strength > 0.0f)
			return false;

		if (pBoundInputInfo->state == InputInfo::State::justReleased)
			justReleasedInputPresent = true;
	}

	return justReleasedInputPresent;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro::InputManager::JoypadAxis fro::InputManager::SDLToJoypadTrigger(Uint8 const SDLAxis)
{
	switch (SDLAxis)
	{
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		return JoypadAxis::leftTrigger;

	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		return JoypadAxis::rightTrigger;

	default:
		throw std::runtime_error("SDL_GameControllerAxis' trigger not mapped to a JoypadAxis");
	}
}

fro::InputManager::JoypadAxis fro::InputManager::SDLToJoypadStick(Sint16 const stickValue, Uint8 const SDLAxis)
{
	switch (SDLAxis)
	{
	case SDL_CONTROLLER_AXIS_LEFTX:
		return stickValue > 0 ? JoypadAxis::leftStickRight : JoypadAxis::leftStickLeft;

	case SDL_CONTROLLER_AXIS_LEFTY:
		return stickValue > 0 ? JoypadAxis::leftStickDown : JoypadAxis::leftStickUp;

	case SDL_CONTROLLER_AXIS_RIGHTX:
		return stickValue > 0 ? JoypadAxis::rightStickRight : JoypadAxis::rightStickLeft;

	case SDL_CONTROLLER_AXIS_RIGHTY:
		return stickValue > 0 ? JoypadAxis::rightStickDown : JoypadAxis::rightStickUp;

	default:
		throw std::runtime_error("SDL_GameControllerAxis' stick not mapped");
	}
}

void fro::InputManager::setInputState(float const newStrength, Input const input)
{
	auto& [strength, state] { m_mInputs[input] };

	state =
		(strength == 0.0f and newStrength > 0.0f) ? InputInfo::State::justPressed :
		(strength > 0.0f and newStrength == 0.0f) ? InputInfo::State::justReleased :
		InputInfo::State::none;

	strength = newStrength;
}
#pragma endregion PrivateMethods