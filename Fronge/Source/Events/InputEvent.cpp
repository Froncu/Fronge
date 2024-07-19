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

    std::string GamepadButtonDownEvent::getLogString() const
    {
        return "gamepad button down event";
    }

    std::string GamepadButtonUpEvent::getLogString() const
    {
        return "gamepad button up event";
    }

    std::string GamepadAxisEvent::getLogString() const
    {
        return "gamepad axis event";
    }
}