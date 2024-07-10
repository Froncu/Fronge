#include "froch.hpp"

#include "IDGenerator.hpp"

namespace fro
{
	IDGenerator::IDGenerator(IDGenerator&& other) noexcept
		: mCounter{ other.mCounter }
		, mFreeIDs{ std::move(other.mFreeIDs) }
	{
		other.mCounter = 0;
	}

	IDGenerator& IDGenerator::operator=(IDGenerator&& other) noexcept
	{
		if (this == &other)
			return *this;

		mCounter = other.mCounter;
		mFreeIDs = std::move(other.mFreeIDs);

		other.mCounter = 0;

		return *this;
	}

	ID IDGenerator::get()
	{
		std::size_t ID;

		++mCounter;

		if (mFreeIDs.empty())
			ID = mCounter;
		else
		{
			ID = *mFreeIDs.begin();
			mFreeIDs.erase(mFreeIDs.begin());
		}

		return { this, ID };
	}
}