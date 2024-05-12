#include "InputManager.h"

#include <sdl.h>
#include <sdl_events.h>

#include <array>
#include <numeric>
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
		info.relativeStrength = 0.0f;
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
			CustomUniquePointer<SDL_GameController>(SDL_GameControllerOpen(event.cdevice.which), SDL_GameControllerClose));
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

void fro::InputManager::bindActionToInput(std::string_view const actionName, Input const input)
{
	m_mActions[actionName].spBoundInputInfos.insert(&m_mInputs[input]);
}

void fro::InputManager::setActionDeadzone(std::string_view const actionName, float const deadzone)
{
	m_mActions[actionName].deadzone = deadzone;
}

float fro::InputManager::getInputStrength(Input const input)
{
	return m_mInputs[input].absoluteStrength;
}

float fro::InputManager::getActionStrength(std::string_view const actionName)
{
	return getActionStrength(actionName, m_mActions[actionName].deadzone);
}

float fro::InputManager::getActionStrengthAxis1D(std::string_view const positiveActionName, std::string_view const negativeActionName)
{
	return getActionStrength(positiveActionName) - getActionStrength(negativeActionName);
}

glm::vec2 fro::InputManager::getActionStrengthAxis2D(std::string_view const positiveActionNameX,
	std::string_view const negativeActionNameX,
	std::string_view const positiveActionNameY,
	std::string_view const negativeActionNameY)
{
	glm::vec2 actionAxis2D
	{ 
		getActionStrength(positiveActionNameX, 0.0f) - getActionStrength(negativeActionNameX, 0.0f),
		getActionStrength(positiveActionNameY, 0.0f) - getActionStrength(negativeActionNameY, 0.0f)
	};

	std::array<float, 4> const aActionDeadzones
	{
		m_mActions[positiveActionNameX].deadzone,
		m_mActions[positiveActionNameY].deadzone,
		m_mActions[positiveActionNameY].deadzone,
		m_mActions[negativeActionNameY].deadzone
	};

	float const avarageDeadzoneMagnitude
	{
		std::reduce(aActionDeadzones.begin(), aActionDeadzones.end(), 0.0f) / aActionDeadzones.size()
	};

	float const actionAxis2DMagnitude{ glm::length(actionAxis2D) };

	if (actionAxis2DMagnitude <= avarageDeadzoneMagnitude)
		return {};

	if (actionAxis2DMagnitude > 1.0f)
		return actionAxis2D / actionAxis2DMagnitude;

	float const inverseLerpedLength{ (actionAxis2DMagnitude - avarageDeadzoneMagnitude) / (1.0f - avarageDeadzoneMagnitude) };
	return actionAxis2D * (inverseLerpedLength / actionAxis2DMagnitude);
}

bool fro::InputManager::isInputJustPressed(Input const input)
{
	auto const& [absoluteStrength, relativeStrength] { m_mInputs[input] };
	return relativeStrength > 0.0f and absoluteStrength == relativeStrength;
}

bool fro::InputManager::isActionJustPressed(std::string_view const actionName)
{
	auto const& [spBoundInputInfos, deadzone] { m_mActions[actionName] };

	bool justCrossedDeadzoneInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
		if (pBoundInputInfo->absoluteStrength > deadzone)
			if (pBoundInputInfo->absoluteStrength - pBoundInputInfo->relativeStrength <= deadzone)
				justCrossedDeadzoneInputPresent = true;
			else
				return false;

	return justCrossedDeadzoneInputPresent;
}

bool fro::InputManager::isInputJustReleased(Input const input)
{
	auto const& [absoluteStrength, relativeStrength] { m_mInputs[input] };
	return relativeStrength < 0.0f and absoluteStrength == 0.0f;
}

bool fro::InputManager::isActionJustReleased(std::string_view const actionName)
{
	auto const& [spBoundInputInfos, deadzone] { m_mActions[actionName] };

	bool justCrossedDeadzoneInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
	{
		if (pBoundInputInfo->absoluteStrength > deadzone)
			return false;

		if (pBoundInputInfo->absoluteStrength - pBoundInputInfo->relativeStrength > deadzone)
			justCrossedDeadzoneInputPresent = true;
	}

	return justCrossedDeadzoneInputPresent;
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
	auto& [absoluteStrength, relativeStrength] { m_mInputs[input] };

	relativeStrength = newStrength - absoluteStrength;
	absoluteStrength = newStrength;
}

float fro::InputManager::getActionStrength(std::string_view const actionName, float const deadzone)
{
	float largestStrength{};
	for (InputInfo const* const pBoundInputInfo : m_mActions[actionName].spBoundInputInfos)
	{
		float const deadzonedStrength
		{
			pBoundInputInfo->absoluteStrength <= deadzone ?
			0.0f :
			(pBoundInputInfo->absoluteStrength - deadzone) / (1.0f - deadzone)
		};

		if (deadzonedStrength > largestStrength)
		{
			largestStrength = deadzonedStrength;
			if (largestStrength == 1.0f)
				break;
		};
	}

	return largestStrength;
}
#pragma endregion PrivateMethods