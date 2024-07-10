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

		std::size_t& counter{ mGenerator->mCounter };
		auto& freeIDs{ mGenerator->mFreeIDs };

		if (mID not_eq counter)
			freeIDs.insert(mID);
		else
		{
			auto highestFreeIDIterator{ freeIDs.end() };
			while (highestFreeIDIterator not_eq freeIDs.begin())
			{
				if (--mID not_eq *--highestFreeIDIterator)
					break;

				freeIDs.erase(highestFreeIDIterator);
				--counter;
			}
		}

		--counter;
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
}