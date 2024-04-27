#if not defined fro_INPUT_MANAGER_H
#define fro_INPUT_MANAGER_H

#include "Singleton.hpp"
#include "ButtonInput.h"
#include "Command.hpp"
#include "Defines.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>

union SDL_Event;

namespace fro
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		virtual ~InputManager() override;

		void processGamePadInputContinous() const;
		void processKeyboardInputContinous() const;
		void processInputEvent(SDL_Event const& event) const;

		template<typename CommandType, typename... Arguments>
			requires std::derived_from<CommandType, Command>
		CommandType& bindActionToCommand(std::string const& actionName, Arguments&&... arguments)
		{
			CommandType* const pCommand{ new CommandType(std::forward<Arguments>(arguments))... };
			m_mCommands[actionName].emplace_back(std::unique_ptr<CommandType>(pCommand));
			return *pCommand;
		}

		void bindKeyInputToAction(ButtonInput keyInput, std::string const& actionName);

	private:
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		std::map<ButtonInput, std::string> m_mActions{};
		std::map<std::string, std::vector<std::unique_ptr<Command>>> m_mCommands{};

		class GameControllerInputImplementation;
		std::unique_ptr<GameControllerInputImplementation> m_pImplementation;
	};
}

#endif