#if not defined ID_HPP
#define ID_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class IDGenerator;

	class ID final
	{
		friend IDGenerator;

	public:
		static std::size_t constexpr INVALID_ID{ std::numeric_limits<std::size_t>::max() };

		FRO_API ID(ID&& other) noexcept;

		~ID();

		FRO_API ID& operator=(ID&& other) noexcept;
		FRO_API FRO_NODISCARD operator std::size_t() const;
		FRO_API FRO_NODISCARD bool operator==(ID const& other) const;
		FRO_API FRO_NODISCARD std::strong_ordering operator<=>(ID const& other) const;

		FRO_API FRO_NODISCARD Reference<IDGenerator> getGenerator() const;

	private:
		ID(Reference<IDGenerator> const generator, std::size_t const ID);
		ID(ID const&) = delete;

		ID& operator=(ID const&) = delete;

		Reference<IDGenerator> mGenerator;
		std::size_t mID;
	};
}

template <>
struct std::formatter<fro::ID>
{
	constexpr auto parse(std::format_parse_context const& ctx) const
	{
		return ctx.begin();
	}

	auto format(fro::ID const& ID, std::format_context& ctx) const
	{
		if (ID == fro::ID::INVALID_ID)
			return std::format_to(ctx.out(), "{}", "INVALID");

		return std::format_to(ctx.out(), "{}", static_cast<std::size_t>(ID));
	}
};

#endif