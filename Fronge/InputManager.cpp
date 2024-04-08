#include "InputManager.h"

#include <SDL2/SDL_events.h>

#pragma region PublicMethods
void fro::InputManager::bindKeyInputToAction(ButtonInput keyInput, const std::string& actionName)
{
	m_mActions.insert({ keyInput, actionName });
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::InputManager::processKeyboardInputContinous() const
{
	auto pKeyboardState{ SDL_GetKeyboardState(nullptr) };
	for (const auto& [buttonInput, actionName] : m_mActions)
	{
		const auto& key{ buttonInput.getButton<SDL_Scancode>() };
		if (!key.has_value())
			continue;

		if (!pKeyboardState[key.value()])
			continue;

		const auto commandIterator{ m_mCommands.find(actionName) };
		if (commandIterator == m_mCommands.end())
			continue;

		for (auto& pCommand : commandIterator->second)
			(*pCommand)();
	}
}

void fro::InputManager::processInputEvent(const SDL_Event& event) const
{
	const auto& eventType{ event.type };

	switch (eventType)
	{
	case SDL_KEYDOWN:
		if (event.key.repeat)
			return;

		[[fallthrough]];

	case SDL_KEYUP:
	{
		const ButtonInput keyInput
		{
			event.key.keysym.scancode,
			eventType == SDL_KEYDOWN ? ButtonInput::State::pressed : ButtonInput::State::released
		};

		const auto actionIterator{ m_mActions.find(keyInput) };
		if (actionIterator == m_mActions.end())
			return;

		const auto commandIterator{ m_mCommands.find(actionIterator->second) };
		if (commandIterator == m_mCommands.end())
			return;

		for (auto& pCommand : commandIterator->second)
			(*pCommand)();
	}
	}
}
#pragma endregion PrivateMethods