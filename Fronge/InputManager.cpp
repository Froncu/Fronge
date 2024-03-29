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
	int numberOfKeys;
	auto pKeyboardState{ SDL_GetKeyboardState(&numberOfKeys) };

	for (int scancode{ 1 }; scancode < numberOfKeys; ++scancode)
	{
		const ButtonInput keyInput{ static_cast<SDL_Scancode>(scancode), ButtonInput::State::down };

		if (pKeyboardState[scancode])
			if (const auto actionsIterator{ m_mActions.find(keyInput) }; actionsIterator != m_mActions.end())
				if (const auto commandsIterator{ m_mCommands.find(actionsIterator->second) }; commandsIterator != m_mCommands.end())
					for (auto& pCommand : commandsIterator->second)
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

		if (const auto actionsIterator{ m_mActions.find(keyInput) }; actionsIterator != m_mActions.end())
			if (const auto commandsIterator{ m_mCommands.find(actionsIterator->second) }; commandsIterator != m_mCommands.end())
				for (auto& pCommand : commandsIterator->second)
					(*pCommand)();
	}
	}
}
#pragma endregion PrivateMethods