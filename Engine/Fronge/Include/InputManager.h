#if not defined fro_INPUT_MANAGER_H
#define fro_INPUT_MANAGER_H

#include "Defines.hpp"
#include "Typenames.hpp"
#include "Singleton.hpp"

#include <glm.hpp>
#include <sdl_scancode.h>
#include <sdl_gamecontroller.h>

#include <map>
#include <set>
#include <variant>
#include <vector>
#include <xstring>

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
			right = 3
		};

		template<typename InputType>
		struct JoypadInput
		{
			JoypadInput(Sint32 deviceID, InputType input)
				: deviceID{ deviceID }
				, input{ input }
			{
			}

			Sint32 deviceID;
			InputType input;

			fro_NODISCARD bool operator<(JoypadInput const& other) const
			{
				return
					deviceID == other.deviceID ?
					input < other.input :
					deviceID < other.deviceID;
			}
		};

		enum class JoypadAxis
		{
			leftStickLeft,
			leftStickRight,
			leftStickUp,
			leftStickDown,
			rightStickLeft,
			rightStickRight,
			rightStickUp,
			rightStickDown,
			leftTrigger,
			rightTrigger
		};

		using Input = std::variant<SDL_Scancode, MouseButton, JoypadInput<SDL_GameControllerButton>, JoypadInput<JoypadAxis>>;

		InputManager();

		virtual ~InputManager() override;

		void processInputContinous();
		void processInputEvent(SDL_Event const& event);
		void bindActionToInput(std::string_view const, Input const input);

		void setActionDeadzone(std::string_view const, float const deadzone);

		fro_NODISCARD float getInputStrength(Input const input);
		fro_NODISCARD float getActionStrength(std::string_view const actionName);
		fro_NODISCARD float getActionStrengthAxis1D(std::string_view const positiveActionName, std::string_view const negativeActionName);
		fro_NODISCARD glm::vec2 getActionStrengthAxis2D(std::string_view const positiveActionNameX,
			std::string_view const negativeActionNameX,
			std::string_view const positiveActionNameY,
			std::string_view const negativeActionNameY);
		fro_NODISCARD bool isInputJustPressed(Input const input);
		fro_NODISCARD bool isActionJustPressed(std::string_view const actionName);
		fro_NODISCARD bool isInputJustReleased(Input const input);
		fro_NODISCARD bool isActionJustReleased(std::string_view const actionName);

	private:
		struct InputInfo final
		{
			float absoluteStrength;
			float relativeStrength;
		};

		struct ActionInfo final
		{
			std::set<InputInfo const*> spBoundInputInfos;
			float deadzone;
		};

		static JoypadAxis SDLToJoypadTrigger(Uint8 const SDLAxis);
		static JoypadAxis SDLToJoypadStick(Sint16 const stickValue, Uint8 const SDLAxis);

		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		void setInputState(float const newStrength, Input const input);

		fro_NODISCARD float getActionStrength(std::string_view const actionName, float const deadzone);

		// TODO: only reason for this is to open and close connected and disconnected joypads; not sure if this is needed
		std::map<SDL_JoystickID, SDLUniquePointer<SDL_GameController>> m_mpJoypads{};
		// END TODO
		std::map<SDL_JoystickID, Sint32> m_mpJoypadInstanceDeviceIDs{};
		std::map<Input, InputInfo> m_mInputs{};
		std::map<std::string_view, ActionInfo> m_mActions{};
	};
}

#endif