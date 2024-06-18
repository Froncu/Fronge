#include "froch.hpp"

#include "BaseEvent.hpp"

namespace fro
{
	std::string BaseEvent::getLogString() const
	{
		return mName.data();
	}

	bool BaseEvent::isInCategory(CategoryType const categoryType) const
	{
		return mCategoryTypes & static_cast<Bitfield>(categoryType);
	}
}