#if not defined UTILITY_HPP
#define UTILITY_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<typename... Types>
	struct VariantVisitor : public Types...
	{
		using Types::operator()...;
	};

	template<typename ResourceType>
	using CustomUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

	using Bitfield = int;
	FRO_NODISCARD static constexpr Bitfield bitfield(std::uint8_t const bitToSet)
	{
		return 1 << bitToSet;
	}
}

#endif