#include "BaseReference.h"

#include "BaseReferencable.h"

#pragma region Constructors/Destructor
fro::BaseReference::BaseReference(BaseReferencable* const pReferencable)
	: m_pReferencable{ pReferencable }
{
	if (valid())
		m_pReferencable->m_spReferences.insert(this);
}

fro::BaseReference::BaseReference(BaseReferencable& referencable)
	: BaseReference(&referencable)
{
}

fro::BaseReference::BaseReference(BaseReference const& other)
	: BaseReference(other.m_pReferencable)
{
}

fro::BaseReference::BaseReference(BaseReference&& other) noexcept
	: BaseReference(other)
{
	other.reset();
}

fro::BaseReference::~BaseReference()
{
	if (valid())
		m_pReferencable->m_spReferences.erase(this);
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::BaseReference& fro::BaseReference::operator=(BaseReference const& other)
{
	if (*this == other)
		return *this;

	if (valid())
		m_pReferencable->m_spReferences.erase(this);

	m_pReferencable = other.m_pReferencable;

	if (valid())
		m_pReferencable->m_spReferences.insert(this);

	return *this;
}

fro::BaseReference& fro::BaseReference::operator=(BaseReference&& other) noexcept
{
	if (this == &other)
		return *this;

	*this = other;

	other.reset();

	return *this;
}

bool fro::BaseReference::operator==(BaseReferencable const* const pReferencable) const
{
	return m_pReferencable == pReferencable;
}

bool fro::BaseReference::operator==(BaseReferencable const& referencable) const
{
	return BaseReference::operator==(&referencable);
}

bool fro::BaseReference::operator==(BaseReference const& other) const
{
	return BaseReference::operator==(other.m_pReferencable);
}

std::strong_ordering fro::BaseReference::operator<=>(BaseReferencable const* const pReferencable) const
{
	return m_pReferencable <=> pReferencable;
}

std::strong_ordering fro::BaseReference::operator<=>(BaseReferencable const& referencable) const
{
	return BaseReference::operator<=>(&referencable);
}

std::strong_ordering fro::BaseReference::operator<=>(BaseReference const& other) const
{
	return BaseReference::operator<=>(other.m_pReferencable);
}
#pragma endregion Operators



#pragma region PublicMethods
void fro::BaseReference::reset()
{
	if (valid())
	{
		m_pReferencable->m_spReferences.erase(this);
		m_pReferencable = nullptr;
	}
}

bool fro::BaseReference::valid() const
{
	return getReferencable();
}
#pragma endregion PublicMethods



#pragma region ProtectedMethods
fro::BaseReferencable* fro::BaseReference::getReferencable() const
{
	return m_pReferencable;
}
#pragma endregion ProtectedMethods