#include "InputManager.h"

#include <SDL2/SDL_events.h>

#pragma region StaticDataMembers
std::map<fro::KeyInput, std::string> fro::InputManager::m_mACTIONS{};
std::map<std::string, std::vector<std::unique_ptr<fro::Command>>> fro::InputManager::m_mCOMMANDS{};
#pragma endregion StaticDataMembers



#pragma region PublicMethods
void fro::InputManager::bindKeyInputToAction(KeyInput keyInput, const std::string& actionName)
{
	m_mACTIONS.insert({ keyInput, actionName });
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::InputManager::processInputContinous()
{
	int numberOfKeys;
	auto pKeyboardState{ SDL_GetKeyboardState(&numberOfKeys) };

	for (size_t scancode{ 1 }; scancode < numberOfKeys; ++scancode)
	{
		const KeyInput keyInput{ static_cast<SDL_Scancode>(scancode), KeyInput::State::down };

		if (pKeyboardState[scancode])
			if (const auto actionsIterator{ m_mACTIONS.find(keyInput) }; actionsIterator != m_mACTIONS.end())
				if (const auto commandsIterator{ m_mCOMMANDS.find(actionsIterator->second) }; commandsIterator != m_mCOMMANDS.end())
					for (auto& pCommand : commandsIterator->second)
						(*pCommand)();
	}
}

void fro::InputManager::processInputEvent(const SDL_Event& event)
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
		const KeyInput keyInput
		{
			event.key.keysym.scancode,
			eventType == SDL_KEYDOWN ? KeyInput::State::pressed : KeyInput::State::released
		};

		if (const auto actionsIterator{ m_mACTIONS.find(keyInput) }; actionsIterator != m_mACTIONS.end())
			if (const auto commandsIterator{ m_mCOMMANDS.find(actionsIterator->second) }; commandsIterator != m_mCOMMANDS.end())
				for (auto& pCommand : commandsIterator->second)
					(*pCommand)();
	}
	}
}
#pragma endregion PrivateMethods