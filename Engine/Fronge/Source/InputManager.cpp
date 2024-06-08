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



#pragma region Operators
bool fro::InputManager::SimulateActionStrengthEvent::operator==(SimulateActionStrengthEvent const& other) const
{
	return
		pActionInfo == other.pActionInfo and
		simulatedStrength == other.simulatedStrength;
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::InputManager::processInputContinous()
{
	for (auto& [input, info] : m_mInputs)
		info.relativeStrength = 0.0f;

	m_SimulateActionStrengthEvents.processAllEvents();

	for (auto& [name, info] : m_mActions)
		if (info.simulatedAbsoluteStrength or info.simulatedRelativeStrength)
			m_SimulateActionStrengthEvents.pushEvent(
				{
					.pActionInfo{ &info },
					.simulatedStrength{ 0.0f }
				});
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

void fro::InputManager::bindActionToInput(std::string const& actionName, Input const input)
{
	m_mActions[actionName].spBoundInputInfos.insert(&m_mInputs[input]);
}

void fro::InputManager::setActionDeadzone(std::string const& actionName, float const deadzone)
{
	m_mActions[actionName].deadzone = deadzone;
}

void fro::InputManager::simulateActionStrength(std::string const& actionName, float const strength)
{
	ActionInfo& actionInfo{ m_mActions[actionName] };

	SimulateActionStrengthEvent event
	{
		.pActionInfo{ &actionInfo },
		.simulatedStrength{ strength }
	};

	m_SimulateActionStrengthEvents.overridePushIf(std::move(event),
		[&actionInfo](SimulateActionStrengthEvent const& queuedEvent)
		{
			return
				queuedEvent.pActionInfo == &actionInfo and
				queuedEvent.simulatedStrength == 0.0f;
		});
}

float fro::InputManager::getInputStrength(Input const input)
{
	return m_mInputs[input].absoluteStrength;
}

float fro::InputManager::getActionStrength(std::string const& actionName)
{
	return getActionStrength(actionName, m_mActions[actionName].deadzone);
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
	glm::vec2 const actionAxis2D
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

	auto const [isOverDeadzone, didJustCrossDeadzone] { getStrengthInfo(absoluteStrength, relativeStrength, 0.0f) };
	return isOverDeadzone and didJustCrossDeadzone;
}

bool fro::InputManager::isActionJustPressed(std::string const& actionName)
{
	auto const& [spBoundInputInfos, simulatedAbsoluteStrength, simulatedRelativeStrength, deadzone]
		{
			m_mActions[actionName]
		};

	bool justCrossedDeadzoneInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
	{
		auto const [isOverDeadzone, didJustCrossDeadzone]
			{
				getStrengthInfo(pBoundInputInfo->absoluteStrength, pBoundInputInfo->relativeStrength, deadzone)
			};

		if (isOverDeadzone)
			if (didJustCrossDeadzone)
				justCrossedDeadzoneInputPresent = true;
			else
				return false;
	}

	auto const [isOverDeadzone, didJustCrossDeadzone]
		{
			getStrengthInfo(simulatedAbsoluteStrength, simulatedRelativeStrength, deadzone)
		};

	if (isOverDeadzone)
		if (didJustCrossDeadzone)
			justCrossedDeadzoneInputPresent = true;
		else
			return false;

	return justCrossedDeadzoneInputPresent;
}

bool fro::InputManager::isInputJustReleased(Input const input)
{
	auto const& [absoluteStrength, relativeStrength] { m_mInputs[input] };

	auto const [isOverDeadzone, didJustCrossDeadzone] { getStrengthInfo(absoluteStrength, relativeStrength, 0.0f) };
	return not isOverDeadzone and didJustCrossDeadzone;
}

bool fro::InputManager::isActionJustReleased(std::string const& actionName)
{
	auto const& [spBoundInputInfos, simulatedAbsoluteStrength, simulatedRelativeStrength, deadzone]
		{
			m_mActions[actionName]
		};

	bool justCrossedDeadzoneInputPresent{};
	for (InputInfo const* const pBoundInputInfo : spBoundInputInfos)
	{
		auto const [isOverDeadzone, didJustCrossDeadzone]
			{
				getStrengthInfo(pBoundInputInfo->absoluteStrength, pBoundInputInfo->relativeStrength, deadzone)
			};

		if (isOverDeadzone)
			return false;

		if (didJustCrossDeadzone)
			justCrossedDeadzoneInputPresent = true;
	}

	auto const [isOverDeadzone, didJustCrossDeadzone]
		{
			getStrengthInfo(simulatedAbsoluteStrength, simulatedRelativeStrength, deadzone)
		};

	if (isOverDeadzone)
		return false;

	if (didJustCrossDeadzone)
		justCrossedDeadzoneInputPresent = true;

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

fro_NODISCARD std::pair<bool, bool> fro::InputManager::getStrengthInfo
(float const absoluteStrength, float const relativeStrength, float const deadzone)
{
	bool const isOverDeadzone{ absoluteStrength >= deadzone };
	float const previousAbsoluteStrength{ absoluteStrength - relativeStrength };

	return
	{
		isOverDeadzone,
		isOverDeadzone ?
		previousAbsoluteStrength < deadzone :
		previousAbsoluteStrength >= deadzone
	};
}

fro_NODISCARD float fro::InputManager::getLargestStrength(float const strength, float const largestStrength, float const deadzone)
{
	float const deadzonedStrength
	{
		strength <= deadzone ?
		0.0f :
		(strength - deadzone) / (1.0f - deadzone)
	};

	return std::max(deadzonedStrength, largestStrength);
}

void fro::InputManager::setInputState(float const newStrength, Input const input)
{
	auto& [absoluteStrength, relativeStrength] { m_mInputs[input] };

	relativeStrength = newStrength - absoluteStrength;
	absoluteStrength = newStrength;
}

float fro::InputManager::getActionStrength(std::string const& actionName, float const deadzone)
{
	float largestStrength{};
	ActionInfo const& actionInfo{ m_mActions[actionName] };

	largestStrength = getLargestStrength(actionInfo.simulatedAbsoluteStrength, largestStrength, deadzone);
	if (largestStrength == 1.0f)
		return largestStrength;

	for (InputInfo const* const pBoundInputInfo : m_mActions[actionName].spBoundInputInfos)
	{
		largestStrength = getLargestStrength(pBoundInputInfo->absoluteStrength, largestStrength, deadzone);
		if (largestStrength == 1.0f)
			return largestStrength;
	}

	return largestStrength;
}
#pragma endregion PrivateMethods