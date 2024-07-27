#if not defined ENTITY_HPP
#define ENTITY_HPP

#include "Reference/Reference.hpp"
#include "Idenitifer/IDGenerator.hpp"

namespace fro
{
	class Entity final : public Referencable
	{
	public:
		FRO_API FRO_NODISCARD static std::size_t getHighestTakenID();

		FRO_API Entity();
		Entity(Entity const&) = default;
		Entity(Entity&&) = default;

		FRO_API ~Entity();

		Entity& operator=(Entity const&) = default;
		Entity& operator=(Entity&&) = default;
		FRO_API FRO_NODISCARD operator std::size_t() const;

		FRO_API FRO_NODISCARD ID const& getID() const;

	private:
		static IDGenerator sIDGenerator;

		ID mID{ sIDGenerator.get() };
	};
}

#endif