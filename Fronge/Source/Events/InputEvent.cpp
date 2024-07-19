#include "froch.hpp"

#include "InputEvent.hpp"

namespace fro
{
	std::string fro::MouseButtonDownEvent::getLogString() const
	{
		return "mouse button down event";
	}

	std::string MouseButtonUpEvent::getLogString() const
	{
		return "mouse button up event";
	}

	std::string KeyDownEvent::getLogString() const
	{
		return "key down event";
	}

	std::string KeyUpEvent::getLogString() const
	{
		return "key up event";
	}

	std::string GamepadConnectedEvent::getLogString() const
	{
		return std::format("gamepad with device ID {} connected event",
			deviceID);
	}

	std::string GamepadDisconnectedEvent::getLogString() const
	{
		return std::format("gamepad with instance ID {} disconnected event",
			instanceID);
	}

	std::string GamepadButtonDownEvent::getLogString() const
	{
		return std::format("gamepad with instance ID {} button down event",
			input.first);
	}

	std::string GamepadButtonUpEvent::getLogString() const
	{
		return std::format("gamepad with instance ID {} button up event",
			input.first);
	}

	std::string GamepadAxisEvent::getLogString() const
	{
		return std::format("gamepad with instance ID {} axis event",
			input.first);
	}
}