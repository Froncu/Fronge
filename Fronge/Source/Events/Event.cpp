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

}