#if not defined INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "InputManager/Input/Input.hpp"
#include "Events/Systems/Event.hpp"

namespace fro
{
	struct MouseButtonDownEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		MouseButton const button;
	};

	struct MouseButtonUpEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		MouseButton const button;
	};

	struct KeyDownEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		Key const key;
	};

	struct KeyUpEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		Key key;
	};

	struct GamepadButtonDownEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadButton const button;
	};

	struct GamepadButtonUpEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadButton const button;
	};

	struct GamepadAxisEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadAxis const axis;
		float const value;
	};

	using InputEvent = EventVariant<
		MouseButtonDownEvent,
		MouseButtonUpEvent,
		KeyDownEvent,
		KeyUpEvent,
		GamepadButtonDownEvent,
		GamepadButtonUpEvent,
		GamepadAxisEvent>;
}

#endif