#pragma once

#include "ButtonInput.h"
#include "Command.h"

#include <SDL2/SDL_keycode.h>
#include <map>
#include <string>
#include <vector>
#include <memory>

union SDL_Event;

namespace fro
{
	class InputManager final
	{
		friend class EventManager;

	public:
		template<typename CommandType, typename... Arguments>
			requires std::derived_from<CommandType, Command>
		static CommandType& bindActionToCommand(const std::string& actionName, Arguments&&... arguments)
		{
			CommandType* const pCommand{ new CommandType(std::forward<Arguments>(arguments))... };
			m_mCOMMANDS[actionName].emplace_back(std::unique_ptr<CommandType>(pCommand));
			return *pCommand;
		}

		static void bindKeyInputToAction(ButtonInput keyInput, const std::string& actionName);

	private:
		InputManager() = delete;
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		~InputManager() = delete;

		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		static void processInputContinous();
		static void processInputEvent(const SDL_Event& event);

		static std::map<ButtonInput, std::string> m_mACTIONS;
		static std::map<std::string, std::vector<std::unique_ptr<Command>>> m_mCOMMANDS;
	};
}