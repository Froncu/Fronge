#include "froch.hpp"

#include "Event.hpp"

namespace fro
{
	std::string Event::getLogString() const
	{
		return mName.data();
	}

	bool Event::isInCategory(CategoryType const categoryType) const
	{
		return mCategoryTypes & static_cast<Bitfield>(categoryType);
	}

	void Event::markHandeled()
	{
		mHandeled = true;
	}

	bool Event::isHandeled() const
	{
		return mHandeled;
	}
}