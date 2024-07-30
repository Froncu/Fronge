#include "froch.hpp"

#include "BaseReference.hpp"

#include "Referencable.hpp"

namespace fro
{
	BaseReference::BaseReference(Referencable const* const pReferencable)
		: mReferencable{ const_cast<Referencable*>(pReferencable) }
	{
		if (valid())
			mReferencable->mReferences.insert(this);
	}

	BaseReference::BaseReference(Referencable const& referencable)
		: BaseReference(&referencable)
	{
	}

	BaseReference::BaseReference(BaseReference const& other)
		: BaseReference(other.mReferencable)
	{
	}

	BaseReference::BaseReference(BaseReference&& other) noexcept
		: BaseReference(other)
	{
		other.reset();
	}

	BaseReference::~BaseReference()
	{
		if (valid())
			mReferencable->mReferences.erase(this);
	}

	BaseReference& BaseReference::operator=(BaseReference const& other)
	{
		if (*this == other)
			return *this;

		if (valid())
			mReferencable->mReferences.erase(this);

		mReferencable = other.mReferencable;

		if (valid())
			mReferencable->mReferences.insert(this);

		return *this;
	}

	BaseReference& BaseReference::operator=(BaseReference&& other) noexcept
	{
		if (this == &other)
			return *this;

		*this = other;

		other.reset();

		return *this;
	}

	bool BaseReference::operator==(Referencable const& referencable) const
	{
		if (not valid())
			return false;

		return *mReferencable == referencable;
	}

	bool BaseReference::operator==(BaseReference const& other) const
	{
		return BaseReference::operator==(other.mReferencable);
	}

	std::strong_ordering BaseReference::operator<=>(Referencable const& referencable) const
	{
		return *mReferencable <=> referencable;
	}

	std::strong_ordering BaseReference::operator<=>(BaseReference const& other) const
	{
		return BaseReference::operator<=>(other.mReferencable);
	}

	void BaseReference::reset()
	{
		if (valid())
		{
			mReferencable->mReferences.erase(this);
			mReferencable = nullptr;
		}
	}

	bool BaseReference::valid() const
	{
		return mReferencable;
	}
}