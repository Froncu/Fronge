#if not defined INPUT_EVENT_HPP
#define INPUT_EVENT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Input/Input.hpp"
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

		Key const key;
	};

	struct GamepadConnectedEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::int32_t const deviceID;
	};

	struct GamepadDisconnectedEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::int32_t const ID;
	};

	struct GamepadButtonDownEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadButtonInput const input;
	};

	struct GamepadButtonUpEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadButtonInput const input;
	};

	struct GamepadAxisEvent final
	{
	public:
		FRO_API FRO_NODISCARD std::string getLogString() const;

		GamepadAxisInput const input;
		double const value;
	};

	using InputEvent = EventVariant<
		MouseButtonDownEvent,
		MouseButtonUpEvent,
		KeyDownEvent,
		KeyUpEvent,
		GamepadConnectedEvent,
		GamepadDisconnectedEvent,
		GamepadButtonDownEvent,
		GamepadButtonUpEvent,
		GamepadAxisEvent>;
}

#endif