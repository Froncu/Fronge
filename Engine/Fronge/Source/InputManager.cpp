#include "InputManager.h"

#include <sdl.h>
#include <sdl_events.h>

#include <algorithm>

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
		setInputStrength(eventType == SDL_KEYDOWN,
			event.key.keysym.scancode);
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		setInputStrength(eventType == SDL_MOUSEBUTTONDOWN,
			static_cast<MouseButton>(event.button.button));
		break;

	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
		setInputStrength(eventType == SDL_CONTROLLERBUTTONDOWN,
			JoypadInput(event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button)));
		break;

	case SDL_CONTROLLERAXISMOTION:
		setInputStrength(static_cast<float>(event.caxis.value) / std::numeric_limits<decltype(event.caxis.value)>::max(),
			JoypadInput(event.caxis.which, static_cast<SDL_GameControllerAxis>(event.caxis.axis)));
		break;
	}
}

void fro::InputManager::bindActionToInput(std::string const& actionName, Input const input)
{
	m_mActions[actionName].insert(input);
}

float fro::InputManager::getInputStrength(Input const input)
{
	return getInputStrengths(input).first;
}

float fro::InputManager::getActionStrength(std::string const& actionName)
{
	auto const& sBoundInputs{ m_mActions[actionName] };

	float largestStrength{};
	for (Input const boundInput : sBoundInputs)
	{
		if (float const boundInputStrength{ getInputStrength(boundInput) };
			boundInputStrength > largestStrength)
		{
			largestStrength = boundInputStrength;
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
	return
	{
		getActionStrengthAxis1D(positiveActionNameX, negativeActionNameX),
		getActionStrengthAxis1D(positiveActionNameY, negativeActionNameY)
	};
}

float fro::InputManager::getInputRelativeStrength(Input const input)
{
	return getInputStrengths(input).second;
}

float fro::InputManager::getActionRelativeStrength(std::string const& /*actionName*/)
{
	// TODO: to be implemented
	return 0.0f;
	// END TODO
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::InputManager::setInputStrength(float const newStrength, Input const input)
{
	auto& inputStrengths{ getInputStrengths(input) };
	inputStrengths.second = newStrength - inputStrengths.first;
	inputStrengths.first = newStrength;
}

std::pair<float, float>& fro::InputManager::getInputStrengths(Input const input)
{
	if (std::holds_alternative<SDL_Scancode>(input))
		return m_mKeys[std::get<SDL_Scancode>(input)];

	else if (std::holds_alternative<MouseButton>(input))
		return m_mMouseButtons[std::get<MouseButton>(input)];

	else if (std::holds_alternative<JoypadInput<SDL_GameControllerButton>>(input))
		return m_mJoypadButtons[std::get<JoypadInput<SDL_GameControllerButton>>(input)];

	else
		return m_mJoypadAxis[std::get<JoypadInput<SDL_GameControllerAxis>>(input)];
}
#pragma endregion PrivateMethods