#include "froch.hpp"

#include "IDGenerator.hpp"

namespace fro
{
	IDGenerator::IDGenerator(IDGenerator&& other) noexcept
		: mHighestTakenID{ other.mHighestTakenID }
		, mFreeIDs{ std::move(other.mFreeIDs) }
	{
		other.mHighestTakenID = 0;
	}

	IDGenerator& IDGenerator::operator=(IDGenerator&& other) noexcept
	{
		if (this == &other)
			return *this;

		mHighestTakenID = other.mHighestTakenID;
		mFreeIDs = std::move(other.mFreeIDs);

		other.mHighestTakenID = 0;

		return *this;
	}

	ID IDGenerator::get()
	{
		std::size_t ID;

		if (mFreeIDs.empty())
			ID = ++mHighestTakenID;
		else
		{
			ID = *mFreeIDs.begin();
			mFreeIDs.erase(mFreeIDs.begin());
		}

		return { this, ID };
	}
}