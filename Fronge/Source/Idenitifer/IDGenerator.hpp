#if not defined ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Reference.hpp"
#include "Idenitifer/ID.hpp"

namespace fro
{
	class IDGenerator final : public Referencable
	{
		friend ID;

	public:
		IDGenerator() = default;
		FRO_API IDGenerator(IDGenerator&& other) noexcept;

		~IDGenerator() = default;

		FRO_API IDGenerator& operator=(IDGenerator&& other) noexcept;

		FRO_API FRO_NODISCARD ID get();

	private:
		IDGenerator(IDGenerator const&) = delete;

		IDGenerator& operator=(IDGenerator const&) = delete;

		std::size_t mHighestTakenID{};
		std::set<std::size_t> mFreeIDs{};
	};
}

#endif