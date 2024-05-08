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
			left = 1,
			middle = 2,
			right = 3,
			wheelUp,
			wheelDown
		};

		template<typename InputType>
		struct JoypadInput
		{
			JoypadInput(int ID, InputType input)
				: ID{ ID }
				, input{ input }
			{
			}

			SDL_JoystickID ID;
			InputType input;

			fro_NODISCARD bool operator<(JoypadInput const& other) const
			{
				return
					ID == other.ID ?
					input < other.input :
					ID < other.ID;
			}
		};

		using Input = std::variant<SDL_Scancode, MouseButton, JoypadInput<SDL_GameControllerButton>, JoypadInput<SDL_GameControllerAxis>>;

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

		std::map<Input, std::pair<float, float>> m_mInputs{};
		std::map<std::string, std::set<Input>> m_mActions{};
	};
}

#endif