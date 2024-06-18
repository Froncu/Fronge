#include "froch.hpp"

#include "WindowEvent.hpp"

namespace fro
{
	WindowEvent::WindowEvent(std::string_view const name, Type const type, std::uint32_t const ID)
		: Event(name, type, CategoryType::SYSTEM, CategoryType::WINDOW)
		, mID{ ID }
	{
	}

	WindowEvent::WindowEvent(WindowEvent const& other)
		: Event(other)
		, mID{ other.mID }
	{
	}

	WindowEvent::WindowEvent(WindowEvent&& other) noexcept
		: Event(std::move(other))
		, mID{ other.mID }
	{
	}

	std::string WindowEvent::getLogString() const
	{
		return std::format("{} with ID{}", mName, mID);
	}
}