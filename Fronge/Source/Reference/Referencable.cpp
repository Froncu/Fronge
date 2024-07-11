#include "froch.hpp"

#include "BaseReference.hpp"
#include "Referencable.hpp"

namespace fro
{
	Referencable::Referencable(Referencable&& other) noexcept
		: mReferences{ std::move(other.mReferences) }
	{
		for (BaseReference* const reference : mReferences)
			reference->mReferencable = this;
	}

	Referencable::~Referencable()
	{
		for (BaseReference* const reference : mReferences)
			reference->mReferencable = nullptr;
	}

	Referencable& Referencable::operator=(Referencable&& other) noexcept
	{
		for (BaseReference* const reference : other.mReferences)
			reference->mReferencable = this;

		mReferences.merge(other.mReferences);

		return *this;
	}
}