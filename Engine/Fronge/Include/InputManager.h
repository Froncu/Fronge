#if not defined fro_INPUT_MANAGER_H
#define fro_INPUT_MANAGER_H

#include "Defines.hpp"
#include "Singleton.hpp"

#include <glm.hpp>
#include <sdl_scancode.h>
#include <sdl_gamecontroller.h>

#include <map>
#include <string>
#include <set>
#include <variant>
#include <vector>

union SDL_Event;

namespace fro
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class MouseButton
		{
			left,
			right,
			middle,
			wheelUp,
			wheelDown
		};

		using Input = std::variant<SDL_Scancode, MouseButton, SDL_GameControllerButton, SDL_GameControllerAxis>;

		InputManager();

		virtual ~InputManager() override;

		void processInputEvent(SDL_Event const& event);
		void bindActionToInput(std::string const& actionName, Input const input);

		fro_NODISCARD float getInputStrength(Input const input);
		fro_NODISCARD float getActionStrength(std::string const& actionName);
		fro_NODISCARD float getActionStrengthAxis1D(std::string const& positiveActionName, std::string const& negativeActionName);
		fro_NODISCARD glm::vec2 getActionStrengthAxis2D(std::string const& positiveActionNameX,
			std::string const& negativeActionNameX,
			std::string const& positiveActionNameY,
			std::string const& negativeActionNameY);
		fro_NODISCARD float getInputRelativeStrength(Input const input);
		fro_NODISCARD float getActionRelativeStrength(std::string const& actionName);

	private:
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		void setInputStrength(float const newStrength, Input const input);

		std::pair<float, float>& getInputStrengths(Input const input);

		std::map<SDL_Scancode, std::pair<float, float>> m_mKeys{};
		std::map<MouseButton, std::pair<float, float>> m_mMouseButtons{};
		std::map<SDL_GameControllerButton, std::pair<float, float>> m_mJoypadButtons{};
		std::map<SDL_GameControllerAxis, std::pair<float, float>> m_mJoypadAxis{};
		std::map<std::string, std::set<Input>> m_mActions{};
	};
}

#endif