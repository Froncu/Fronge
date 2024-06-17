#if not defined EVENT_HPP
#define EVENT_HPP

#include "Core.hpp"
#include "Utility.hpp"

#include <string>

namespace fro
{
	class Event
	{
	public:
		enum class FRO_API Type
		{
			WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_UNFOCUS, WINDOW_MOVE,

			KEY_PRESS, KEY_RELEASE,

			BUTTON_PRESSED, BUTTON_RELEASED,

			MOUSE_MOVE, MOUSE_SCROLL
		};

		enum class FRO_API CategoryType
		{
			SYSTEM	= bitfield(0),
			WINDOW	= bitfield(1),
			INPUT	= bitfield(2),
			KEY		= bitfield(3),
			BUTTON	= bitfield(4),
			MOUSE	= bitfield(5),
		};

		FRO_API virtual ~Event() = default;

		FRO_API FRO_NODISCARD virtual std::string getLogString() const;
		FRO_API FRO_NODISCARD bool isInCategory(CategoryType const categoryFlag) const;

		std::string_view const mName;
		Type const mType;
		Bitfield const mCategoryTypes;

	protected:
		template<std::same_as<CategoryType> ...Bits>
		Event(std::string_view const name, Type const type, Bits const... categoryTypes)
			: mName{ name }
			, mType{ type }
			, mCategoryTypes{ (static_cast<Bitfield>(categoryTypes) | ...) }
		{
		}

		Event(Event const&) = default;
		Event(Event&&) noexcept = default;

	private:
		Event& operator=(Event const&) = delete;
		Event& operator=(Event&&) noexcept = delete;
	};
}

#endif