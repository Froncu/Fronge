#include "froch.hpp"

#include "ID.hpp"
#include "IDGenerator.hpp"

namespace fro
{
	ID::ID(Reference<IDGenerator> const generator, std::size_t const ID)
		: mGenerator{ generator }
		, mID{ ID }
	{
	}

	ID::ID(ID&& other) noexcept
		: mGenerator{ std::move(other.mGenerator) }
		, mID{ other.mID }
	{
		other.mID = INVALID_ID;
	}

	ID::~ID()
	{
		if (not mGenerator.valid())
			return;

		std::size_t& highestTakenID{ mGenerator->mHighestTakenID };
		auto& freeIDs{ mGenerator->mFreeIDs };

		if (mID not_eq highestTakenID)
			freeIDs.insert(mID);
		else
		{
			--highestTakenID;

			auto endIterator{ freeIDs.end() };
			while (endIterator not_eq freeIDs.begin())
			{
				auto const highestFreeIDIterator{ --freeIDs.end() };
				if (highestTakenID not_eq *highestFreeIDIterator)
					break;

				freeIDs.erase(highestFreeIDIterator);

				--highestTakenID;
				endIterator = freeIDs.end();
			}
		}
	}

	ID& ID::operator=(ID&& other) noexcept
	{
		if (this == &other)
			return *this;

		mGenerator = std::move(other.mGenerator);
		mID = other.mID;

		other.mID = INVALID_ID;

		return *this;
	}

	ID::operator std::size_t() const
	{
		return mID;
	}

	Reference<IDGenerator> ID::getGenerator() const
	{
		return mGenerator;
	}
}