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

	case SDL_MOUSEWHEEL:
		// TODO: add support for handling mouse wheel input
		
		// END TODO
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
	return m_mInputs[input].first;
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

// TODO: the relative input strength is never reset to zero in case no relative change was registered; needs to be fixed
float fro::InputManager::getInputRelativeStrength(Input const input)
{
	return m_mInputs[input].second;
}
// END TODO

// TODO: to be implemented
float fro::InputManager::getActionRelativeStrength(std::string const& /*actionName*/)
{
	return 0.0f;
}
// END TODO
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::InputManager::setInputStrength(float const newStrength, Input const input)
{
	auto& inputStrengths{ m_mInputs[input] };
	inputStrengths.second = newStrength - inputStrengths.first;
	inputStrengths.first = newStrength;
}
#pragma endregion PrivateMethods