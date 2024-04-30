#include "InputManager.h"

#include <sdl_events.h>

#pragma region PublicMethods
void fro::InputManager::processKeyboardInputContinous() const
{
	auto pKeyboardState{ SDL_GetKeyboardState(nullptr) };
	for (auto const& [buttonInput, actionName] : m_mActions)
	{
		if (buttonInput.getState() not_eq ButtonInput::State::down)
			continue;

		auto const& key{ buttonInput.getButton<SDL_Scancode>() };
		if (not key.has_value())
			continue;

		if (not pKeyboardState[key.value()])
			continue;

		auto const commandIterator{ m_mCommands.find(actionName) };
		if (commandIterator == m_mCommands.end())
			continue;

		for (auto& pCommand : commandIterator->second)
			(*pCommand)();
	}
}

void fro::InputManager::processInputEvent(SDL_Event const& event) const
{
	auto const& eventType{ event.type };

	switch (eventType)
	{
	case SDL_KEYDOWN:
		if (event.key.repeat)
			return;

		[[fallthrough]];

	case SDL_KEYUP:
	{
		ButtonInput const keyInput
		{
			event.key.keysym.scancode,
			eventType == SDL_KEYDOWN ? ButtonInput::State::pressed : ButtonInput::State::released
		};

		auto const actionIterator{ m_mActions.find(keyInput) };
		if (actionIterator == m_mActions.end())
			return;

		auto const commandIterator{ m_mCommands.find(actionIterator->second) };
		if (commandIterator == m_mCommands.end())
			return;

		for (auto& pCommand : commandIterator->second)
			(*pCommand)();
	}
	}
}

void fro::InputManager::bindKeyInputToAction(ButtonInput keyInput, std::string const& actionName)
{
	m_mActions.insert({ keyInput, actionName });
}
#pragma endregion PublicMethods